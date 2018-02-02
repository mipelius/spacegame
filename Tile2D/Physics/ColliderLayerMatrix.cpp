// This file is part of SpaceGame.
// Copyright (C) 2018 Miika Pelkonen
//
// SpaceGame is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// SpaceGame is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SpaceGame.  If not, see <http://www.gnu.org/licenses/>.


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
