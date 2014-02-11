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
#include "Map.h"
#include "Rect.h"
#include "MapTexture.h"
#include "Body.h"
#include "CollisionShape.h"

Map::Map(
        std::string path,
        int blockSizeW,
        int blockSizeH
) {
    this->blockSizeW = blockSizeW;
    this->blockSizeH = blockSizeH;

    using namespace std;

    SDL_Surface* surface = IMG_Load(path.data());
    if (!surface) {
        std::fprintf(stderr, "Error during loading map: %s\n", SDL_GetError());
        exit(1);
    }

    Uint8* pixels = (Uint8*)surface->pixels;

    this->initialize(surface->w, surface->h);

    for (int i=0; i<surface->w; i++) {
        for (int j=0; j<surface->h; j++) {
            this->setValue(i, j, *(pixels + j * w + i));
        }
    }

    SDL_FreeSurface(surface);
}

void Map::initialize(int w, int h) {
    this->w = w;
    this->h = h;
    p_values = new unsigned char*[w];

    for (int i = 0; i < w; i++) {
        p_values[i] = new unsigned char[h];
        unsigned char *row = p_values[i];
        for (int j = 0; j < h;j++) {
            row[j] = 0;
        }
    }
}

void Map::setValue(int x, int y, unsigned char value) {
    if (x < 0 || x >= w || y < 0 || y >= h) return;
    unsigned char *row = p_values[x];
    row[y] = value;
}

unsigned char Map::getValue(int x, int y) {
    if (x < 0 || x >= w || y < 0 || y >= h) return 0;
    unsigned char *row = p_values[x];
    return row[y];
}

int Map::getW() {
    return w;
}

int Map::getH() {
    return h;
}

long Map::getActualW() {
    return w * blockSizeW;
}

long Map::getActualH() {
    return h * blockSizeH;
}

unsigned char Map::getValueActual(int x, int y) {
    return this->getValue(x / blockSizeW, y / blockSizeH);
}

void Map::setValueActual(int x, int y, unsigned char value) {
    this->setValue(x / blockSizeW, y / blockSizeH, value);
}

bool Map::detectCollisionWith(Body *body) {
    if (!body->getCollisionShape()) return false;

    if (this->getValueActual((int) body->location->get().x, (int) body->location->get().y)) return true;

    Point* points = body->getCollisionShape()->getRotatedPoints();
    for (int i=0; i< body->getCollisionShape()->getCount(); i++) {
        int x = (int)(points[i].x + body->location->get().x);
        int y = (int)(points[i].y + body->location->get().y);
        if (this->getValueActual(x, y)) return true;
    }

    Rect boundingBox = body->getCollisionShape()->getBoundingBox();

    int iBegin = (int)boundingBox.x1 - ((int)boundingBox.x1) % blockSizeW;
    int iEnd = (int)boundingBox.x2 + (int)boundingBox.x2 % blockSizeW;
    int jBegin = (int)boundingBox.y1 - ((int)boundingBox.y1) % blockSizeH;
    int jEnd = (int)boundingBox.y2 + (int)boundingBox.y2 % blockSizeH;

    for (int i=iBegin; i <= iEnd; i += blockSizeW) {
        for (int j=jBegin; j <= jEnd ; j += blockSizeH) {
            if (this->getValueActual(i, j)) {
                Rect* rect = new Rect(i, j, i + blockSizeW, j + blockSizeH);
                if (body->getCollisionShape()->intersectsWith(rect)) {
                    return true;
                }
            }
        }
    }

    return false;
}

int Map::getBlockW() {
    return this->blockSizeW;
}


int Map::getBlockH() {
    return this->blockSizeH;
}
