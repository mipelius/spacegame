//
// Created by Miika Pelkonen on 12/15/13.
// Copyright (c) 2013 ___MIIKAPELKONEN___. All rights reserved.
//

#include "Map.h"
#include <iostream>
#include <fstream>

Map::Map(char *path) {
    using namespace std;

    ifstream file(path, ios::in|ios::binary|ios::beg);

    // this is only temporary implementation.

    if (file.is_open())
    {
        // the size is fixed: 2000 x 2000
        short W = 4000;
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

char* Map::getColumn(int x) {
    return (char *)p_values[x];
}

short Map::getW() {
    return w;
}

short Map::getH() {
    return h;
}

