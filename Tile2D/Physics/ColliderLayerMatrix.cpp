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
#include "ColliderLayerMatrix.h"

ColliderLayerMatrix::ColliderLayerMatrix(std::vector<Rule> rules)
{
    numLayers_ = 0;
    for (auto rule : rules) {
    if (rule.colliderLayer1 >= numLayers_) {
            numLayers_ = rule.colliderLayer1 + 1;
        }
        if (rule.colliderLayer2 >= numLayers_) {
            numLayers_ = rule.colliderLayer2 + 1;
        }
    }
    layerRules_ = new Array2d<int>(numLayers_, numLayers_);
    layerRules_->fill(-1); // -1 stands for "unset"

    for (auto rule : rules) {
        int currentRule = rule.collides ? 1 : 0;
        layerRules_->setValue(rule.colliderLayer1, rule.colliderLayer2, currentRule);
        layerRules_->setValue(rule.colliderLayer2, rule.colliderLayer1, currentRule);
    }
}

ColliderLayerMatrix::~ColliderLayerMatrix() {
    delete layerRules_;
}

bool ColliderLayerMatrix::getRule(unsigned int layer, unsigned otherLayer) {
    int rule = -1;
    if (layerRules_->isInsideBounds(layer, otherLayer)) {
        rule = layerRules_->getValue(layer, otherLayer);
    }

    if (rule == -1) {
        std::string error = "No rule for collider layers ";
        error += std::to_string(layer);
        error += " and ";
        error += std::to_string(otherLayer);
        error += ".";
        throw std::runtime_error(error);
    }

    return rule != 0;
}

unsigned int ColliderLayerMatrix::getNumLayers() const {
    return numLayers_;
}
