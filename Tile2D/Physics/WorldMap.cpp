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

#include "precompile.h"
#include "WorldMap.h"
#include "Body.h"
#include "CollisionShape.h"
#include "BlockMapping.h"
#include "WorldMapModifiedEventArgs.h"

WorldMap::WorldMap(
        std::string path,
        BlockMapping &mapping,
        int blockSizeW,
        int blockSizeH
) :
    modification(new Event<WorldMap, WorldMapModifiedEventArgs>(this))
{
    mapping_ = &mapping;

    this->blockSizeW = blockSizeW;
    this->blockSizeH = blockSizeH;

    using namespace std;

    SDL_Surface* surface = IMG_Load(path.data());
    if (!surface) {
        std::fprintf(stderr, "Error during loading map: %s\n", SDL_GetError());
        exit(1);
    }

    Uint8* pixels = (Uint8*)surface->pixels;

    blocks_ = new Array2d<Block*>(surface->w, surface->h);

    for (int i=0; i<surface->w; i++) {
        for (int j=0; j<surface->h; j++) {
            unsigned char id = *(pixels + j * surface->w + i);
            Block* value = mapping.getBlock(id);
            blocks_->setValue(i, j, value);
        }
    }

    SDL_FreeSurface(surface);
}

WorldMap::~WorldMap() {
    delete blocks_;
    delete modification;
}

void WorldMap::setValue(int x, int y, Block* value) {
    if (blocks_->isInsideBounds(x, y)) {
        Block* oldValue = blocks_->getValue(x, y);

        if (oldValue != value) {
            blocks_->setValue(x, y, value);

            WorldMapModifiedEventArgs args;
            args.x = x;
            args.y = y;
            args.oldValue = oldValue;
            args.newValue = value;

            modification->raise(args);
        }
    }
}


Block* WorldMap::getValue(int x, int y) {
    if (blocks_->isInsideBounds(x, y)) {
        return blocks_->getValue(x, y);
    }

    return nullptr;
}

void WorldMap::setValueScaled(Vector point, Block *value) {
    setValue(
            (int)(point.x / blockSizeW),
            (int)(point.y / blockSizeH),
            value
    );
}

Block *WorldMap::getValueScaled(Vector point) {
    return getValue(
            (int)(point.x / blockSizeW),
            (int)(point.y / blockSizeH)
    );
}

int WorldMap::getW() {
    return blocks_->getW();
}

int WorldMap::getH() {
    return blocks_->getH();
}

long WorldMap::getActualW() {
    return blocks_->getW() * blockSizeW;
}

long WorldMap::getActualH() {
    return blocks_->getH() * blockSizeH;
}

bool WorldMap::detectCollisionWith(Body& body) {
    Block* block = this->getValueScaled(body.position.get());

    if (block != nullptr && block->density.get() > 0.0) return true;

    const Vector* points = body.getCollisionShape().getRotatedPoints();

    for (int i=0; i< body.getCollisionShape().getCount(); i++) {
        Vector point(
                points[i].x + body.position.get().x,
                points[i].y + body.position.get().y
        );

        block = this->getValueScaled(point);
        if (block != nullptr && block->density.get() > 0.0) return true;
    }

    Rect boundingBox = body.getCollisionShape().getBoundingBox();

    int iBegin = (int)boundingBox.x1 - ((int)boundingBox.x1) % blockSizeW;
    int iEnd = (int)boundingBox.x2 + (int)boundingBox.x2 % blockSizeW;
    int jBegin = (int)boundingBox.y1 - ((int)boundingBox.y1) % blockSizeH;
    int jEnd = (int)boundingBox.y2 + (int)boundingBox.y2 % blockSizeH;

    for (int i=iBegin; i <= iEnd; i += blockSizeW) {
        for (int j=jBegin; j <= jEnd ; j += blockSizeH) {
            block = this->getValueScaled(Vector(i, j));
            if (block != nullptr && block->density.get() > 0.0) {
                Rect rect = Rect(i, j, i + blockSizeW, j + blockSizeH);
                if (body.getCollisionShape().intersectsWith(rect)) {
                    return true;
                }
            }
        }
    }

    return false;
}

int WorldMap::getBlockW() {
    return this->blockSizeW;
}


int WorldMap::getBlockH() {
    return this->blockSizeH;
}