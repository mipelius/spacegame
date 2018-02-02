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


#ifndef SPACEGAME_COLLIDERLAYERMATRIX_H
#define SPACEGAME_COLLIDERLAYERMATRIX_H


#include <stdexcept>
#include <vector>
#include "Array2d.h"

class ColliderLayerMatrix {
public:
    struct Rule {
        unsigned int    colliderLayer1;
        unsigned int    colliderLayer2;
        bool            collides;
    };

    ColliderLayerMatrix(std::vector<Rule> rules);
    ~ColliderLayerMatrix();

    bool getRule(unsigned int layer, unsigned otherLayer);
    unsigned int getNumLayers() const;


private:
    Array2d<int>* layerRules_;
    unsigned int numLayers_;
};


#endif //SPACEGAME_COLLIDERLAYERMATRIX_H
