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


#ifndef __WorldMap_H_
#define __WorldMap_H_

#include <string>
#include "Array2d.h"
#include "DrawableMap.h"
#include "TileSet.h"

class MapTexture;
class Body;
class TileSet;
class Tile;
class WorldMapModifiedEventArgs;

#include "Rect.h"
#include "Tile.h"
#include "Event.h"

class TileMap {
    friend class Canvas;

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

    bool canMove(
            const Veci &position,
            const Veci &boundingBoxTopLeftCorner,
            const Veci &boundingBoxBottomRightCorner
    );

    bool canMoveScaled(const Vecf &pos, const Rect& boundingBox, bool useInnerBounding = false);

    void load(
            std::string path,
            std::string tileSet
    );

    void unload();
    bool isLoaded();

    TileSet* getTileSet();

    TileMap();
    ~TileMap();

    bool castLine(const Vecf &start, const Vecf &goal, Vecf &collisionPoint);

    inline Veci getMapCoords(Vecf worldCoords) {
        return {
                (int)(worldCoords.x / tileSet_->getTileW()),
                (int)(worldCoords.y / tileSet_->getTileH())
        };
    }

    inline Vecf getWorldCoords(Veci mapCoords) {
        return {
                (mapCoords.x + 0.5f) * tileSet_->getTileW(),
                (mapCoords.y + 0.5f) * tileSet_->getTileH()
        };
    };

private:
    static const int DEFAULT_BLOCK_SIZE_W = 8;
    static const int DEFAULT_BLOCK_SIZE_H = 8;
    static const int MAX_COLLISION_BOUNDING_BOX_AREA = 40000;

    DrawableMap* drawableMap_ = nullptr;
    Array2d<Tile*> *tiles_ = nullptr;
    TileSet* tileSet_ = nullptr;

};

#endif //__WorldMap_H_
