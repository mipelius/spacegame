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

#ifndef __WorldMap_H_
#define __WorldMap_H_

#include <string>

class MapTexture;
class Body;
class TileSet;
class Tile;
class WorldMapModifiedEventArgs;

#include "Rect.h"
#include "Array2d.h"
#include "Tile.h"
#include "Event.h"

class TileMap {

public:
    TileMap(
            std::string path,
            TileSet &tileSet,
            int blockSizeW = TileMap::DEFAULT_BLOCK_SIZE_W,
            int blockSizeH = TileMap::DEFAULT_BLOCK_SIZE_H
    );

    ~TileMap();

    Event<TileMap, WorldMapModifiedEventArgs>* const modification;

    Tile* getValue(int x, int y);
    Tile* getValueScaled(Vec point);
    void setValue(int x, int y, Tile* value);
    void setValueScaled(Vec point, Tile* value);

    int getW();
    int getH();
    int getBlockW();
    int getBlockH();
    long getActualW();
    long getActualH();

    bool detectCollisionWith(Body& body);

private:
    static const int DEFAULT_BLOCK_SIZE_W = 8;
    static const int DEFAULT_BLOCK_SIZE_H = 8;
    static const int MAX_COLLISION_BOUNDING_BOX_AREA = 40000;
    int blockSizeW;
    int blockSizeH;

    Array2d<Tile*> *blocks_;
    TileSet* mapping_;
};

#endif //__WorldMap_H_
