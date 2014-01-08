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
#include <iostream>
#include <fstream>
#include <SDL2/SDL_opengl.h>

Map::Map(char *path, int blockSizeW, int blockSizeH) {
    this->blockSizeW = blockSizeW;
    this->blockSizeH = blockSizeH;

    using namespace std;

    ifstream file(path, ios::in|ios::binary|ios::beg);

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
    unsigned char *row = p_values[x];
    row[y] = value;
}

char Map::getValue(int x, int y) {
    unsigned char *row = p_values[x];
    return row[y];
}

short Map::getW() {
    return w;
}

short Map::getH() {
    return h;
}

void Map::render(long x, long y, int w, int h) {
    int iStart = 0;
    int jStart = 0;

    if (x > 0) iStart = x / blockSizeW;
    if (y > 0) jStart = y / blockSizeH;

    if (iStart < 0) iStart = 0;
    if (jStart < 0) jStart = 0;

    int iEnd = this->getW();
    int jEnd = this->getH();

    if (x + w < this->getW() * blockSizeW) iEnd = (x + w) / blockSizeW + 1;
    if (y + h < this->getH() * blockSizeH) jEnd = (y + h) / blockSizeH + 1;

    // TODO: use Block-object instead of unsigned char



    for (int i = iStart; i < iEnd; i++) {
        for (int j = jStart; j < jEnd; j++) {
            if (!this->getValue(i, j)) continue; // continue if the block is empty
            glColor3f(0.2, 0.5, 0.0);

            glBegin(GL_QUADS);

            // TODO: add texture

            //Bottom-left vertex (corner)
            glVertex2i(i * blockSizeW - x, j * blockSizeH - y);

            //Bottom-right vertex (corner)
            glVertex2i(i * blockSizeW - x + blockSizeW, j * blockSizeH - y);

            //Top-right vertex (corner)
            glVertex2i(i * blockSizeW - x + blockSizeW, j * blockSizeH - y + blockSizeH);

            //Top-left vertex (corner)
            glVertex2i(i * blockSizeW - x + 0.f, j * blockSizeH - y + blockSizeH);

            glEnd();

        }
    }

}