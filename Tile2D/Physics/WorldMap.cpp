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
#include "Rect.h"
#include "MapTexture.h"
#include "Body.h"
#include "CollisionShape.h"
#include "BlockMapping.h"

WorldMap::WorldMap(
        std::string path,
        BlockMapping* mapping,
        int blockSizeW,
        int blockSizeH
)
{
    mapping_ = mapping;

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
            Block* value = mapping->getBlock(id);
            blocks_->setValue(i, j, value);
        }
    }

    SDL_FreeSurface(surface);
}

WorldMap::~WorldMap() {
    delete blocks_;
}

void WorldMap::setValue(int x, int y, Block* value) {
    if (blocks_->isInsideBounds(x, y)) {
        blocks_->setValue(x, y, value);
    }
}


Block* WorldMap::getValue(int x, int y) {
    if (blocks_->isInsideBounds(x, y)) {
        return blocks_->getValue(x, y);
    }

    return nullptr;
}

void WorldMap::setValueScaled(Point point, Block *value) {
    setValue(
            (int)(point.x / blockSizeW),
            (int)(point.y / blockSizeH),
            value
    );
}

Block *WorldMap::getValueScaled(Point point) {
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

bool WorldMap::detectCollisionWith(Body *body) {
    if (!body->getCollisionShape()) return false;

    // TODO: make this work ...

//
//    if (this->getByteValueActual((int) body->location->get().x, (int) body->location->get().y)) return true;
//
//    Point* points = body->getCollisionShape()->getRotatedPoints();
//
//    for (int i=0; i< body->getCollisionShape()->getCount(); i++) {
//        int x = (int)(points[i].x + body->location->get().x);
//        int y = (int)(points[i].y + body->location->get().y);
//        if (this->getByteValueActual(x, y)) return true;
//    }
//
//    Rect boundingBox = body->getCollisionShape()->getBoundingBox();
//
//    int iBegin = (int)boundingBox.x1 - ((int)boundingBox.x1) % blockSizeW;
//    int iEnd = (int)boundingBox.x2 + (int)boundingBox.x2 % blockSizeW;
//    int jBegin = (int)boundingBox.y1 - ((int)boundingBox.y1) % blockSizeH;
//    int jEnd = (int)boundingBox.y2 + (int)boundingBox.y2 % blockSizeH;
//
//    for (int i=iBegin; i <= iEnd; i += blockSizeW) {
//        for (int j=jBegin; j <= jEnd ; j += blockSizeH) {
//            if (this->getByteValueActual(i, j)) {
//                Rect* rect = new Rect(i, j, i + blockSizeW, j + blockSizeH);
//                if (body->getCollisionShape()->intersectsWith(rect)) {
//                    return true;
//                }
//            }
//        }
//    }

    return false;
}

int WorldMap::getBlockW() {
    return this->blockSizeW;
}


int WorldMap::getBlockH() {
    return this->blockSizeH;
}