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


#include "Tile2DMath.h"
#include "DrawableMap.h"
#include "TileMap.h"
#include "Tile2D.h"
#include "TileSet.h"
#include "WorldMapModifiedEventArgs.h"
#include "LightSystem.h"

TileMap::TileMap() :
    modification(this),
    tiles_(nullptr)
{ }

TileMap::~TileMap() {
    unload();
}

void TileMap::setValue(int x, int y, Tile* value) {
    if (tiles_->isInsideBounds(x, y)) {
        Tile* oldValue = tiles_->getValue(x, y);

        if (oldValue != value) {
            tiles_->setValue(x, y, value);

            WorldMapModifiedEventArgs args;
            args.x = x;
            args.y = y;
            args.oldValue = oldValue;
            args.newValue = value;

            modification.raise(args);
        }
    }
}


Tile* TileMap::getValue(int x, int y) const {
    if (tiles_->isInsideBounds(x, y)) {
        return tiles_->getValue(x, y);
    }

    return nullptr;
}

void TileMap::setValueScaled(Vecf point, Tile *value) {
    setValue(
            (int)(point.x / tileSet_->getTileW()),
            (int)(point.y / tileSet_->getTileH()),
            value
    );
}

Tile *TileMap::getValueScaled(Vecf point) const {
    return getValue(
            (int)(point.x / tileSet_->getTileW()),
            (int)(point.y / tileSet_->getTileH())
    );
}

int TileMap::getW() const {
    return tiles_->w;
}

int TileMap::getH() const {
    return tiles_->h;
}

long TileMap::getActualW() const {
    return tiles_->w * tileSet_->getTileW();
}

long TileMap::getActualH() const {
    return tiles_->h * tileSet_->getTileH();
}

void TileMap::load(std::string path, std::string tileSet) {
    unload();
    tileSet_ = new TileSet(tileSet);

    SDL_Surface* surface = IMG_Load(path.data());
    if (!surface) {
        std::fprintf(stderr, "Error during loading map: %s\n", SDL_GetError());
        exit(1);
    }

    Uint8* pixels = (Uint8*)surface->pixels;

    tiles_ = new Array2d<Tile*>(surface->w, surface->h);

    for (int i=0; i<surface->w; i++) {
        for (int j=0; j<surface->h; j++) {
            unsigned char id = *(pixels + j * surface->w + i);
            Tile* value = tileSet_->getTile(id);
            tiles_->setValue(i, j, value);
        }
    }

    SDL_FreeSurface(surface);

    drawableMap_ = new DrawableMap();
    drawableMap_->setMap(this);
    drawableMap_->setMapTexture(tileSet_->getMapTexture());
    Tile2D::lightSystem().createLightMap_();
}

void TileMap::unload() {
    if (!isLoaded()) {
        return;
    }
    delete drawableMap_;    drawableMap_ = nullptr;
    delete tileSet_;        tileSet_ = nullptr;
    delete tiles_;          tiles_ = nullptr;
}

bool TileMap::isLoaded() {
    return tiles_ != nullptr;
}

TileSet *TileMap::getTileSet() {
    return tileSet_;
}

bool TileMap::canMove(const Veci &position, const Veci &boundingBoxTopLeftCorner, const Veci &boundingBoxBottomRightCorner) {
    Tile* tile = nullptr;
    Veci currentPos;

    for (auto x = boundingBoxTopLeftCorner.x; x <= boundingBoxBottomRightCorner.x; ++x) {
        for (auto y = boundingBoxTopLeftCorner.y; y <= boundingBoxBottomRightCorner.y; ++y) {
            currentPos = position + Veci(x, y);
            tile = getValue(currentPos.x, currentPos.y);

            if (tile == nullptr || tile->getDensity() > 0.0f) {
                return false;
            }
        }
    }

    return true;
}

bool TileMap::canMoveScaled(const Vecf &position, const Rect& boundingBox, bool useInnerBounding) {
    const int& w = tileSet_->getTileW();
    const int& h = tileSet_->getTileH();

    Veci pos                            = {(int)(position.x   / w)          , (int)(position.y    / h)};

    Veci boundingBoxTopLeftCorner       = {(int)floor(boundingBox.x1 / w)   , (int)floor(boundingBox.y1 / h)};
    Veci boundingBoxBottomRightCorner   = {(int)ceil(boundingBox.x2 / w)    , (int)ceil(boundingBox.y2 / h)};

    if (useInnerBounding) {
        boundingBoxTopLeftCorner        += {1, 1};
        boundingBoxBottomRightCorner    -= {1, 1};
    }

    return canMove(pos, boundingBoxTopLeftCorner, boundingBoxBottomRightCorner);
}

bool TileMap::castLine(const Vecf &start, const Vecf &goal, Vecf &collisionPoint) {
    bool result = false;

    if (!isLoaded()) {
        return result;
    }

    Veci startMapCoords = getMapCoords(start);
    Veci goalMapCoords = getMapCoords(goal);

    if (startMapCoords == goalMapCoords) {
        collisionPoint = getWorldCoords(startMapCoords);
        return true;
    }

    Vecf delta = {
            (float)(goalMapCoords.x - startMapCoords.x),
            (float)(goalMapCoords.y - startMapCoords.y)
    };

    if (fabsf(delta.x) >= fabsf(delta.y)) {
        float slope = delta.y / delta.x;

        if (delta.x < Mathf::epsilon) {
            slope *= -1.0f;
        }

        auto stepX = goalMapCoords.x - startMapCoords.x > 0 ? 1 : -1;

        float y = startMapCoords.y;

        for (auto x = startMapCoords.x; x != goalMapCoords.x; x += stepX) {
            Veci currentPoint = {x, (int)y};
            Tile* tile = getValue(currentPoint.x, currentPoint.y);
            if (tile != nullptr && tile->getDensity() > 0.0f) {
                collisionPoint = getWorldCoords(currentPoint);
                return true;
            }

            y += slope;
        }
    } else {
        float slope = delta.x / delta.y;

        if (delta.y < Mathf::epsilon) {
            slope *= -1.0f;
        }

        auto stepY = goalMapCoords.y - startMapCoords.y > 0 ? 1 : -1;

        float x = startMapCoords.x;

        for (auto y = startMapCoords.y; y != goalMapCoords.y; y += stepY) {
            Veci currentPoint = {(int)x, y};
            Tile* tile = getValue(currentPoint.x, currentPoint.y);
            if (tile != nullptr && tile->getDensity() > 0.0f) {
                collisionPoint = getWorldCoords(currentPoint);
                return true;
            }

            x += slope;
        }
    }

    return result;
}
