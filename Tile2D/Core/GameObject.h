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


#ifndef __GameObject_H
#define __GameObject_H

#include <list>
#include <map>
#include "Transform.h"

class ITile2DComponentReflector;
class Tile2DComponent;

class GameObject {
    friend class Tile2D;
    friend class Prefab;

public:
    int tag = -1;

    template <class T>
    T* getComponent();

    template <class T>
    T* attachComponent();

    void destroy();

    Transform& transform();

    bool isActive() const;
    void setIsActive(bool isActive);

    GameObject* clone();

private:
    explicit GameObject(const json::Object& jsonObject);
    GameObject();
    ~GameObject();

    void deserialize_(const json::Object &jsonObject);

    Transform transform_;

    std::list<Tile2DComponent*> uninitializedComponents_;
    std::list<Tile2DComponent*> components_;

    bool isActive_ = true;
    bool isAlive_ = true;
    bool canBeDestroyed_ = false;

    void prepareDestroy_();
    void initializeComponents_();
    void attachComponentInternal(Tile2DComponent* component);
};

// template function

template <class T>
T* GameObject::getComponent() {
    for (auto component : uninitializedComponents_) {
        auto currentComponent = dynamic_cast<T*>(component);
        if (currentComponent != nullptr) {
            return currentComponent;
        }
    }

    for (auto component : components_) {
        auto currentComponent = dynamic_cast<T*>(component);
        if (currentComponent != nullptr) {
            return currentComponent;
        }
    }

    return nullptr;
}

template<class T>
T *GameObject::attachComponent() {
    auto component = new T();
    attachComponentInternal(component);
    return component;
}

#endif //__GameObject_H
