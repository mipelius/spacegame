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

#include "Map.h"
#include "GameEntity.h"
#include <iostream>
#include <fstream>
#include <SDL2/SDL_opengl.h>

Map::Map(
        std::string path,
        MapTexture* mapTexture,
        int blockSizeW,
        int blockSizeH
) {
    this->blockSizeW = blockSizeW;
    this->blockSizeH = blockSizeH;
    this->mapTexture = mapTexture;

    using namespace std;

    ifstream file(path.data(), ios::in|ios::binary|ios::beg);

    // this is only temporary implementation.

    if (file.is_open())
    {
        // the size is fixed: 2000 x 2000
        short W = 2000;
        short H = 2000;
        this->initialize(W, H);

        file.seekg(1078);

        char currentChar = 0;

        for (int i = H - 1; i >= 0; i--) {
            for (int j = 0; j < W; j++) {
                file.read(&currentChar, 1);
                this->setValue(j, i, (unsigned char)currentChar);
            }
            //file.read(&currentChar, 2);
        }

    }

    file.close();

    // comment the above code and uncomment the code below

//    if (file.is_open())
//    {
//        // read size of the map
//
//        file.read((char*)&w, 2);
//        file.read((char*)&h, 2);
//
//        // allocate memory for the map
//        fprintf(stdout, "sizeX: %d, sizeY: %d\n", w, h);
//
//        initialize(w, h);
//
//        char currentChar = 0;
//
//        for (int i = 0; i < h; i++) {
//            for (int j = 0; j < w; j++) {
//                file.read(&currentChar, 1);
//                this->setValue(j, i, (unsigned char)currentChar);
//            }
//        }
//
//        file.close();
//
//    }
//
//    else cout << "Unable to open file";
}

void Map::initialize(short w, short h) {
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

short Map::getW() {
    return w;
}

short Map::getH() {
    return h;
}

void Map::render(double x, double y, double w, double h) {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    int iStart = 0;
    int jStart = 0;

    if (x > 0) iStart = int(x / blockSizeW);
    if (y > 0) jStart = int(y / blockSizeH);

    if (iStart < 0) iStart = 0;
    if (jStart < 0) jStart = 0;

    int iEnd = this->getW();
    int jEnd = this->getH();

    if (x + w < this->getW() * blockSizeW) iEnd = int((x + w) / blockSizeW + 1);
    if (y + h < this->getH() * blockSizeH) jEnd = int((y + h) / blockSizeH + 1);

    // NOTE: the textureNumber that is used for the current block is return value from getValue(i, j) - 1;
    // Maybe you should use more sophisticated way to indicate the correspondence between mapValues and textureNumbers

    mapTexture->glBind();

    for (int i = iStart; i < iEnd; i++) {
        for (int j = jStart; j < jEnd; j++) {
            if (!this->getValue(i, j)) continue; // continue if the block is empty

            GLdouble color = sin(((i*blockSizeW - x) / (float)w) * (M_PI / 1.0)) * sin(((j*blockSizeH - y) / (float)h) * (M_PI / 1.0));
            color /= 1.5;
            color += 0.25;
            if (color > 1.0) color = 1.0;
            glColor4d(color, color, color, color);

            mapTexture->renderBlock(
                    (int)(i * blockSizeW - x),
                    (int)(j * blockSizeH - y),
                    blockSizeW,
                    blockSizeH,
                    this->getValue(i, j) - 1
            );
        }
    }
    mapTexture->glUnbind();

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

bool Map::detectCollisionWith(GameEntity *entity) {
    if (this->getValueActual(entity->getLocation().x, entity->getLocation().y)) return true;

    Point* points = entity->getCollisionShape()->getRotatedPoints();
    for (int i=0; i<entity->getCollisionShape()->getCount(); i++) {
        int x = (int)(points[i].x + entity->getCollisionShape()->getLocation().x);
        int y = (int)(points[i].y + entity->getCollisionShape()->getLocation().y);
        if (this->getValueActual(x, y)) return true;
    }

    Rectangle boundingBox = entity->getCollisionShape()->getBoundingBox();

    int iBegin = (int)boundingBox.getFirstPoint().x - ((int)boundingBox.getFirstPoint().x) % blockSizeW;
    int iEnd = (int)boundingBox.getSecondPoint().x + (int)boundingBox.getSecondPoint().x % blockSizeW;
    int jBegin = (int)boundingBox.getFirstPoint().y - ((int)boundingBox.getFirstPoint().y) % blockSizeH;
    int jEnd = (int)boundingBox.getSecondPoint().y + (int)boundingBox.getSecondPoint().y % blockSizeH;

    for (int i=iBegin; i <= iEnd; i += blockSizeW) {
        for (int j=jBegin; j <= jEnd ; j += blockSizeH) {
            if (this->getValueActual(i, j)) {
                Rectangle* rect = new Rectangle(Point(i, j), Point(i + blockSizeW, j + blockSizeH));
                if (entity->getCollisionShape()->intersectsWith(rect)) {
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
