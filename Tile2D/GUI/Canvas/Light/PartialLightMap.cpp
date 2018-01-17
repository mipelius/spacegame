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

#include <Tile2D/Util/MathUtils.h>
#include "PartialLightMap.h"
#include "TileMap.h"

PartialLightMap::PartialLightMap(int w, int h) : Array2d<unsigned char>(w, h) {

}

void PartialLightMap::setCenterLocation(int x, int y) {
    int newX = x - (w_ / 2);
    int newY = y - (h_ / 2);
    if (newX != x_ || newY != y_) {
        x_ = x - (w_ / 2);
        y_ = y - (h_ / 2);
        needsUpdate_ = true;
    }
}

void PartialLightMap::update(TileMap* map) {
    updateInternal(255, w_ / 2, h_ / 2, map);
    needsUpdate_ = false;
}

void PartialLightMap::updateInternal(unsigned char lastLight, int currentX, int currentY, TileMap* map) {
    int radius = w_ / 2;
    int x = currentX - radius;
    int y = currentY - radius;

    int length = MathUtils::getLength(x, y);
    int distanceToBorder = radius - length;

    if (distanceToBorder < 1) {
        return;
    }

    int actualX = currentX + x_;
    int actualY = currentY + y_;

    Tile* currentBlock = map->getValue(actualX, actualY);
    if (!currentBlock) return;

    double translucency = currentBlock->translucency.get();
    auto tileReduction = 256 - translucency * 256.0;

    int newLight = lastLight - tileReduction;

    if (newLight <= this->getValue(currentX, currentY)) return;
    if (newLight < 0) return;

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

bool PartialLightMap::needsUpdate() {
    return needsUpdate_;
}
