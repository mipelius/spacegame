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


#include "GameObject.h"
#include "Tile2D.h"
#include "Tile2DComponentReflector.h"

void GameObject::initializeComponents_() {
    for (auto& component : uninitializedComponents_) {
        component->init();
        components_.push_back(component);
    }
    uninitializedComponents_.clear();
}

void GameObject::destroy() {
    isAlive_ = false;
}

GameObject::GameObject() { }

GameObject::~GameObject() {
    for (auto& component : uninitializedComponents_) {
        delete component;
    }
    for (auto& component : components_) {
        delete component;
    }
}

Transform &GameObject::transform() {
    return transform_;
}

bool GameObject::isActive() const {
    return isActive_ && isAlive_;
}

void GameObject::setIsActive(bool isActive) {
    isActive_ = isActive;
}

void GameObject::prepareDestroy_() {
    for (auto& component : components_) {
        component->onDestroy();
    }
    canBeDestroyed_ = true;
}

GameObject::GameObject(
        const json::Object&                                         jsonObject,
        const std::map<std::string, ITile2DComponentReflector *> &  componentBindings
) {
    if (jsonObject.HasKey("transform")) {
        auto jsonTransformObject = jsonObject["transform"];
        transform_.deserialize(jsonTransformObject);
    }

    for (auto& currentComponentJson : jsonObject["components"].ToArray()) {
        auto classTypeString = currentComponentJson["class"].ToString();

        Tile2DComponent* component = nullptr;

        auto it = componentBindings.find(classTypeString);
        auto reflector = (*it).second;

        if (reflector != nullptr) {
            auto componentPropertiesJsonObject = currentComponentJson["properties"];
            component = reflector->create(componentPropertiesJsonObject);
            attachComponentInternal(component);
        }
        else {
            std::string error = "serialization error: no class binding for \"";
            error.append(classTypeString);
            error.append("\"\n");

            throw std::runtime_error(error.data());
        }
    }
}

void GameObject::attachComponentInternal(Tile2DComponent *component) {
    component->gameObject_ = this;
    uninitializedComponents_.push_back(component);
}

