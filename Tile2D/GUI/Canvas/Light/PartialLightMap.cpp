// This file is part of SpaceGame.
// Copyright (C) 2014 Miika Pelkonen
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

#include "PartialLightMap.h"
#include "WorldMap.h"

PartialLightMap::PartialLightMap(int w, int h) : Array2d<unsigned char>(w, h) {

}

void PartialLightMap::setCenterLocation(int x, int y) {
    x_ = x - (w_ / 2);
    y_ = y - (h_ / 2);
}

void PartialLightMap::update(WorldMap* map) {
    updateInternal(255, w_ / 2, h_ / 2, map);
}

void PartialLightMap::updateInternal(unsigned char lastLight, int currentX, int currentY, WorldMap* map) {
    if (!isInsideBounds(currentX, currentY)) {
        return;
    }

    int actualX = currentX + x_;
    int actualY = currentY + y_;

    Block* currentBlock = map->getValue(actualX, actualY);
    if (!currentBlock) return;

    double translucency = currentBlock->translucency->get();

    unsigned newLight = lastLight;

    unsigned char reduction = (unsigned char)(128 - translucency * 128.0);

    if (reduction > 0) {
        if (lastLight == 255) {
            newLight -= 1;
        }
        else {
            newLight -= reduction;
        }
    }

    if (newLight <= this->getValue(currentX, currentY)) return;

    if (newLight < 64) return;

    this->setValue(currentX, currentY, newLight);

    updateInternal(newLight, currentX - 1, currentY    , map);
    updateInternal(newLight, currentX + 1, currentY    , map);
    updateInternal(newLight, currentX    , currentY - 1, map);
    updateInternal(newLight, currentX    , currentY + 1, map);
}

int PartialLightMap::getX() const {
    return x_;
}

int PartialLightMap::getY() const {
    return y_;
}