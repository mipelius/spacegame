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

void GameObject::initializeComponents_() {
    for (auto& component : uninitializedComponents_) {
        component->init();
        components_.push_back(component);
    }
    uninitializedComponents_.clear();
}

void GameObject::destroy() {
    Tile2D::instance_().destroy_(this);
}

GameObject::GameObject() {
    Tile2D::instance_().objectsToInit_.push_back(this);
}

GameObject::~GameObject() {
    for (auto& component : uninitializedComponents_) {
        delete component;
    }
    for (auto& component : components_) {
        component->onDestroy();
    }
    for (auto& component : components_) {
        delete component;
    }
}

Transform &GameObject::transform() {
    return transform_;
}

bool GameObject::isActive() const {
    return isActive_;
}

void GameObject::setIsActive(bool isActive) {
    isActive_ = isActive;
}

