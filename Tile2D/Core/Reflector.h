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

#ifndef SPACEGAME_REFLECTOR_H
#define SPACEGAME_REFLECTOR_H


#include <iostream>
#include "ISerializable.h"
#include "ObjectCreator.h"

class Reflector {
    friend class Tile2D;

public:
    ISerializable* instantiate(const std::string& className);

    template <class T>
    T* instantiate(const json::Object& jsonObject);

private:
    Reflector() = default;
    ~Reflector();

    void init_(std::map<std::string, IObjectCreator*> bindings);

    std::map<std::string, IObjectCreator*> bindings_;
};

template<class T>
T *Reflector::instantiate(const json::Object &jsonObject) {
    auto className = jsonObject["class"].ToString();

    auto serializable = instantiate(className);

    if (jsonObject.HasKey("properties")) {
        if (jsonObject["properties"].GetType() == json::ObjectVal) {
            auto propertiesJson = jsonObject["properties"].ToObject();
            serializable->deserialize(propertiesJson);
        }
    }

    auto object = dynamic_cast<T*>(serializable);

    if (object == nullptr) {
        std::cout << "Reflector: invalid conversion for \"" + className + "\"";
    }

    return object;
}

#endif //SPACEGAME_REFLECTOR_H
