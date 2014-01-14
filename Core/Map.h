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
#include "MapTexture.h"

class Map {
public:
    Map(
            std::string path,
            MapTexture* mapTexture,
            int blockSizeW = Map::DEFAULT_BLOCK_SIZE_W,
            int blockSizeH = Map::DEFAULT_BLOCK_SIZE_H
    );
    void setValue(int x, int y, unsigned char value);
    void setValueActual(int x, int y, unsigned char value);
    unsigned char getValue(int x, int y);
    unsigned char getValueActual(int x, int y);
    short getW();
    short getH();
    long getActualW();
    long getActualH();
    void render(double x, double y, double w, double h);

private:
    MapTexture* mapTexture;
    static const int DEFAULT_BLOCK_SIZE_W = 8;
    static const int DEFAULT_BLOCK_SIZE_H = 8;
    int blockSizeW;
    int blockSizeH;
    short w;
    short h;
    unsigned char **p_values;
    void initialize(short w, short h);
};

#endif //__Map_H_
