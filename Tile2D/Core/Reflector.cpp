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

#include "Reflector.h"

void Reflector::init_(std::map<std::string, IObjectCreator*> bindings) {
    bindings_ = std::move(bindings);
}

Reflector::~Reflector() {
    for (auto binding : bindings_) {
        delete binding.second;
    }
}

ISerializable *Reflector::instantiate(const json::Object &jsonObject) {
    auto className = jsonObject["class"].ToString();
    auto mapResult = bindings_.find(className);
    if (mapResult == bindings_.end()) {
        throw std::runtime_error("Reflector: No class binding for \"" + className + "\".");
    }

    auto objectCreator = (*mapResult).second;
    auto propertiesJson = jsonObject["properties"].ToObject();

    return objectCreator->create(propertiesJson);
}
