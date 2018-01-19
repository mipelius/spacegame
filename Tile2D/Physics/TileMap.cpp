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

#include "DrawableMap.h"
#include "TileMap.h"
#include "Tile2D.h"
#include "Body.h"
#include "WorldMapModifiedEventArgs.h"

TileMap::TileMap() :
    modification(this),
    tiles_(nullptr)
{ }

TileMap::~TileMap() {
    if (tiles_ != nullptr) {
        delete tiles_;
    }
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

void TileMap::setValueScaled(Vec point, Tile *value) {
    setValue(
            (int)(point.x / tileSet_->getTileW()),
            (int)(point.y / tileSet_->getTileH()),
            value
    );
}

Tile *TileMap::getValueScaled(Vec point) const {
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
    Tile2D::physicsWorld().map_ = this;
    Tile2D::lightSystem().makeLightMap_();
}

void TileMap::unload() {
    if (!isLoaded()) {
        return;
    }
    delete drawableMap_;
    delete tileSet_;
    delete tiles_;
    Tile2D::physicsWorld().map_ = nullptr;
}

bool TileMap::isLoaded() {
    return tiles_ != nullptr;
}

TileSet *TileMap::getTileSet() {
    return tileSet_;
}
