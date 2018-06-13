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


#include <Tile2D/Util/JsonFileManager.h>
#include "GameObject.h"
#include "Tile2D.h"
#include "Reflector.h"

void GameObject::initializeComponents_() {
    if (isInitialized_) {
        throw std::runtime_error("GameObject: already initialized!");
    }
    for (auto& component : components_) {
        component->init();
    }
    isInitialized_ = true;
}

void GameObject::destroy() {
    isAlive_ = false;
}

GameObject::GameObject() { }

GameObject::~GameObject() {
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

GameObject::GameObject(const json::Object& jsonObject) {
    struct ComponentInfo {
        ISerializable*  serializable;
        json::Object    propertiesJson;
    };

    std::list<ComponentInfo> componentInfoList;

    // reads jsonData -> creates and attachs components and replaces transform and tag if necessary
    auto createStub = [this] (std::list<ComponentInfo>& componentInfoList, const json::Object& jsonData) {
        if (jsonData.HasKey("tag")) {
            int tagId = jsonData["tag"].ToInt();
            tag_ = &Tile2D::getTag(tagId);
        }
        if (jsonData.HasKey("transform")) {
            transform_.deserialize(jsonData["transform"]);
        }
        if (jsonData.HasKey("components")) {
            for (auto &currentComponentJson : jsonData["components"].ToArray()) {
                auto classString = currentComponentJson["class"].ToString();

                auto serializable = Tile2D::reflector().instantiate(classString);
                auto component = dynamic_cast<Tile2DComponent *>(serializable);

                if (component == nullptr) {
                    std::string error =
                            "GameObject : \"" +
                            currentComponentJson["class"].ToString() +
                            "\" is not bound to a Tile2DComponent.";

                    throw std::runtime_error(error);
                }

                if (currentComponentJson.HasKey("id")) {
                    component->id_ = currentComponentJson["id"].ToInt();
                };

                attachComponentInternal(component);

                auto propertiesJson = currentComponentJson["properties"].ToObject();

                componentInfoList.push_back({serializable, propertiesJson});
            }
        }
    };

    if (jsonObject.HasKey("template")) {
        auto templateJsonObject = jsonObject["template"];
        auto templateFilePath = templateJsonObject["file"].ToString();
        auto templateReplacementJsonObject = templateJsonObject["replacement"];

        auto jsonObjectFromTemplate = JsonFileManager::load(
                templateFilePath,
                templateReplacementJsonObject
        );

        createStub(componentInfoList, jsonObjectFromTemplate);
    }

    createStub(componentInfoList, jsonObject);

    // deserialization phase
    for (auto componentInfo : componentInfoList) {
        auto serializable = componentInfo.serializable;
        const auto& jsonProperties = componentInfo.propertiesJson;

        serializable->deserialize(jsonProperties);
    }
}

void GameObject::attachComponentInternal(Tile2DComponent *component) {
    component->gameObject_ = this;
    components_.push_back(component);
}

GameObject* GameObject::clone() {
    auto gameObject = Tile2D::createGameObject();

    for (auto component : components_) {
        auto cloneComponent = component->clone();
        gameObject->attachComponentInternal(cloneComponent);
    }

    gameObject->transform_ = this->transform_;

    return gameObject;
}

Tile2DComponent *GameObject::getComponent(int id) {
    for (auto component : components_) {
        if (component->id_ == id) {
            return component;
        }
    }

    return nullptr;
}

void GameObject::setTag(const Tag &tag) {
    tag_ = &tag;
}

const Tag &GameObject::getTag() {
    return *tag_;
}
