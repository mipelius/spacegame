//
// Created by Miika Pelkonen on 1/10/14.
// Copyright (c) 2014 ___MIIKAPELKONEN___. All rights reserved.
//


#ifndef __MapTexture_H_
#define __MapTexture_H_

#include <string>
#include <SDL2_image/SDL_image.h>
#include <SDL2/SDL_opengl.h>

class MapTexture {
private:
    GLuint texture;
    int blockW;
    int blockH;
    int count;

    void createGlTexture(SDL_Surface*);

public:
    MapTexture(int blockW, int blockH, int count, std::string fileNames[]);

    void renderBlock(int x, int y, int w, int h, int textureNumber);

    void glUnbind();

    void glBind();
};


#endif //__MapTexture_H_
