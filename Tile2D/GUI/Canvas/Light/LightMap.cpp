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

#include "LightMap.h"
#include "Map.h"

LightMap::LightMap(int w, int h) {
    w_ = w;
    h_ = h;
    data_ = new double[w * h];

    clear();
}

LightMap::~LightMap() {
    delete[] data_;
}

void LightMap::clear() {
    memset(data_, 0, sizeof(double) * w_ * h_);
}

void LightMap::applyLight(int x, int y, Map *map, int offsetX, int offsetY) {
    applyLightInternal(x, y, map, offsetX, offsetY, 1.0);
}

void LightMap::applyLightCenter(Map *map, int offsetX, int offsetY) {
    applyLight(w_ / 2, h_ / 2, map, offsetX, offsetY);
}

void LightMap::applyLightInternal(int currentX, int currentY, Map *map, int offsetX, int offsetY, double lastLight) {
//    double deltaX = currentX * map->getBlockW() - radius_;
//    double deltaY = currentY * map->getBlockH() - radius_;
//
//    double dist = sqrt(deltaX * deltaX + deltaY * deltaY);
//
//    if (dist >= radius_) return;

    if (currentX < 0 || currentX >= w_) return;
    if (currentY < 0 || currentY >= h_) return;

    unsigned char value = map->getValue(offsetX + currentX, offsetY + currentY);
    double newLight = lastLight - (value == 0 ? 0.0 : 0.15);

    int index = currentX + currentY * w_;

    if (newLight <= data_[index]) return;

    data_[index] = newLight;

    applyLightInternal(currentX + 1, currentY, map, offsetX, offsetY, newLight);
    applyLightInternal(currentX, currentY + 1 , map, offsetX, offsetY, newLight);
    applyLightInternal(currentX - 1, currentY, map, offsetX, offsetY, newLight);
    applyLightInternal(currentX, currentY - 1, map, offsetX, offsetY, newLight);
}
