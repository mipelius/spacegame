// MIT License
//
// This file is part of SpaceGame.
// Copyright (c) 2014-2018 Miika Pelkonen
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


#ifndef __Event_H_
#define __Event_H_

#include <list>
#include <functional>
#include "ISerializable.h"
#include "Tile2D.h"
#include "Reflector.h"
#include "json.h"

template <typename TOwner, typename TArgs> class IEventHandler {
public:
    virtual void handle(TOwner* owner, TArgs args) const = 0;
    virtual IEventHandler<TOwner, TArgs>* clone() = 0;
};

template <typename TOwner, typename TArgs>
class EventHandlerWrapper : public IEventHandler<TOwner, TArgs> {
private:
    void (*functionPtr_)(TOwner* owner, TArgs args);

public:
    explicit EventHandlerWrapper(void (*functionPtr)(TOwner* owner, TArgs args)) {
        functionPtr_ = functionPtr;
    }
    void handle(TOwner* owner, TArgs args) const override {
        functionPtr_(owner, args);
    }

    IEventHandler<TOwner, TArgs>* clone() {
        return new EventHandlerWrapper(*this);
    }
};

template <typename TOwner, typename TArgs>
class Event : public ISerializable {
public:
    Event() = default;
    Event(const Event<TOwner, TArgs>& eventToCopy);

    ~Event();

    void raise(TOwner* owner, TArgs eventArgs) const;
    void add(IEventHandler<TOwner, TArgs>* handler);
    void add(void (*functionPtr)(TOwner*, TArgs));

    void deserialize(const json::Object &jsonObject) override;

private:
    std::list< IEventHandler<TOwner, TArgs>* > eventHandlers_;
};

template <typename TOwner, typename TArgs>
void Event<TOwner, TArgs>::raise(TOwner* owner, TArgs eventArgs) const {
    for (auto eventHandler : eventHandlers_) {
        eventHandler->handle(owner, eventArgs);
    }
}

template <typename TOwner, typename TArgs>
void Event<TOwner, TArgs>::add(IEventHandler<TOwner, TArgs>* handler) {
    eventHandlers_.push_back(handler);
}

template<typename TOwner, typename TArgs>
void Event<TOwner, TArgs>::add(void (*functionPtr)(TOwner *, TArgs)) {
    auto handler = new EventHandlerWrapper<TOwner, TArgs>(functionPtr);
    eventHandlers_.push_back(handler);
}

template<typename TOwner, typename TArgs>
Event<TOwner, TArgs>::Event(const Event<TOwner, TArgs>& eventToCopy) {
    for (auto handler : eventToCopy.eventHandlers_) {
        auto handlerClone = handler->clone();
        eventHandlers_.push_back(handlerClone);
    }
}

template<typename TOwner, typename TArgs>
Event<TOwner, TArgs>::~Event() {
    for (auto handler : eventHandlers_) {
        delete handler;
    }
}

template<typename TOwner, typename TArgs>
void Event<TOwner, TArgs>::deserialize(const json::Object &jsonObject) {
    auto handlersJson = jsonObject["handlers"].ToArray();

    for (const auto& handlerJson : handlersJson) {
        const auto& handlerJsonObj = handlerJson.ToObject();
        auto handler = Tile2D::reflector().instantiate< IEventHandler<TOwner, TArgs> >(handlerJsonObj);
        eventHandlers_.push_back(handler);
    }
}


#endif //__Event_H_
