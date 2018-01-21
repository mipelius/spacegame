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
#include "Array2d.h"
#include "DrawableMap.h"

class MapTexture;
class Body;
class TileSet;
class Tile;
class WorldMapModifiedEventArgs;

#include "Rect.h"
#include "Tile.h"
#include "Event.h"

class TileMap {
    friend class Window;

public:
    Event<TileMap, WorldMapModifiedEventArgs> const modification;

    Tile* getValue(int x, int y) const;
    Tile* getValueScaled(Vecf point) const;
    void setValue(int x, int y, Tile* value);
    void setValueScaled(Vecf point, Tile* value);

    int getW() const;
    int getH() const;
    long getActualW() const;
    long getActualH() const;

    void load(
            std::string path,
            std::string tileSet
    );

    void unload();
    bool isLoaded();

    TileSet* getTileSet();

    TileMap();
    ~TileMap();

private:
    static const int DEFAULT_BLOCK_SIZE_W = 8;
    static const int DEFAULT_BLOCK_SIZE_H = 8;
    static const int MAX_COLLISION_BOUNDING_BOX_AREA = 40000;

    DrawableMap* drawableMap_ = nullptr;
    Array2d<Tile*> *tiles_ = nullptr;
    TileSet* tileSet_ = nullptr;
};

#endif //__WorldMap_H_
