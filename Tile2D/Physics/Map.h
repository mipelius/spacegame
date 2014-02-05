// This file is part of SpaceGame.
// Copyright (C) 2014  Miika Pelkonen
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

#ifndef __Map_H_
#define __Map_H_

#include <string>

class MapTexture;
class GameEntity;

#include "Rect.h"

class Map {
public:
    Map(
            std::string path,
            int blockSizeW = Map::DEFAULT_BLOCK_SIZE_W,
            int blockSizeH = Map::DEFAULT_BLOCK_SIZE_H
    );

    void setValue(int x, int y, unsigned char value);
    void setValueActual(int x, int y, unsigned char value);
    unsigned char getValue(int x, int y);
    unsigned char getValueActual(int x, int y);
    int getW();
    int getH();
    int getBlockW();
    int getBlockH();
    long getActualW();
    long getActualH();

    bool detectCollisionWith(GameEntity * entity);

    void renderSmall(Rect mapRect, Rect renderingAreaRect);

private:
    static const int DEFAULT_BLOCK_SIZE_W = 8;
    static const int DEFAULT_BLOCK_SIZE_H = 8;
    int blockSizeW;
    int blockSizeH;
    int w;
    int h;
    unsigned char **p_values;
    void initialize(int w, int h);
};

#endif //__Map_H_
