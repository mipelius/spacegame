//
// Created by Miika Pelkonen on 1/10/14.
// Copyright (c) 2014 ___MIIKAPELKONEN___. All rights reserved.
//


#include "Texture.h"

#ifndef __Background_H_
#define __Background_H_


class Background {
private:
    Texture* texture;
    int x;
    int y;
    int w;
    int h;
    void init(Texture *texture, int x, int y, int w, int h);
public:
    Background(Texture *texture, int x, int y);
    Background(Texture *texture, int x, int y, int w, int h);

    void render(int offSetX, int offSetY, int w, int h);
    int getX();
    int getY();
    int getW();
    int getH();

};


#endif //__Background_H_
