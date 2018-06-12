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

#include <string>
#include "JsonFileManager.h"
#include "ColliderLayerMatrix.h"

ColliderLayerMatrix::ColliderLayerMatrix(const std::string &colliderLayersFile)
{
    auto jsonObject = JsonFileManager::load(colliderLayersFile);

    auto colliderLayersJson = jsonObject["colliderLayers"].ToArray();
    auto colliderRulesJson = jsonObject["rules"].ToArray();

    int maxId = -1;
    for (const auto& colliderLayerJson : colliderLayersJson) {
        int id              = colliderLayerJson["id"].ToInt();
        std::string name    = colliderLayerJson["name"].ToString();

        colliderLayerMap_[id] = {id, name};

        if (maxId < id) {
            maxId = id;
        }
    }

    layerRules_ = new Array2d<int>(maxId + 1, maxId + 1);
    layerRules_->fill(-1); // -1 stands for "unset"

    for (const auto& ruleJson : colliderRulesJson) {
        auto idFirst    = ruleJson.ToArray()[0].ToInt();
        auto idSecond   = ruleJson.ToArray()[1].ToInt();
        auto collides   = ruleJson.ToArray()[2].ToBool();

        int currentRule = collides ? 1 : 0;
        layerRules_->setValue(idFirst, idSecond, currentRule);
        layerRules_->setValue(idSecond, idFirst, currentRule);
    }
}

ColliderLayerMatrix::~ColliderLayerMatrix() {
    delete layerRules_;
}

bool ColliderLayerMatrix::getRule(const ColliderLayer& layer1, const ColliderLayer& layer2) const {
    int rule = -1;

    if (layerRules_->isInsideBounds(layer1.id, layer2.id)) {
        rule = layerRules_->getValue(layer1.id, layer2.id);
    }

    if (rule == -1) {
        std::string error = "No rule for collider layers";
        error += " { " + std::to_string(layer1.id) + ", \"" + layer1.name +"\" } ";
        error += "and";
        error += " { " + std::to_string(layer2.id) + ", \"" + layer2.name +"\" } ";
        error += ".";
        throw std::runtime_error(error);
    }

    return rule != 0;
}

const ColliderLayer &ColliderLayerMatrix::getColliderLayer(int id) const {
    auto it = colliderLayerMap_.find(id);
    if (it == colliderLayerMap_.end()) {
        throw std::runtime_error("ColliderLayerMatrix: no collider layer for id " + std::to_string(id));
    }

    return (*it).second;
}
