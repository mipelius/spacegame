//
// Created by Miika Pelkonen on 1/10/14.
// Copyright (c) 2014 ___MIIKAPELKONEN___. All rights reserved.
//

#include "MapTexture.h"

MapTexture::MapTexture(int blockW, int blockH, int count, std::string fileNames []) {
    this->blockW = blockW;
    this->blockH = blockH;
    this->count = count;

    SDL_Surface* dstSurface = SDL_CreateRGBSurface(
            0,
            blockW * count,
            blockH,
            32,
            0xFF000000,
            0x00FF0000,
            0x0000FF00,
            0x000000FF
    );

    Uint32* dstPixels = (Uint32*)dstSurface->pixels;

    for (int i=0; i<count; i++) {
        SDL_Surface* srcSurface = IMG_Load(fileNames[i].data());
        if (!srcSurface) {
            fprintf(stderr, "SDL could not load %s: %s\n", fileNames[i].data(), SDL_GetError());
            SDL_Quit();
            exit(1);
        }
        if (srcSurface->w != blockW || srcSurface->h != blockH) {
            fprintf(stderr, "Image has invalid size (%s)\n", fileNames[i].data());
            exit(1);
        }

        Uint32* srcPixels = (Uint32*)srcSurface->pixels;

        for (int col=0; col<blockW; col++) {
            for (int row=0; row<blockH; row++) {
                Uint32* dstCurrentPixel = dstPixels + row * (blockW * count) + i * blockW + col;
                Uint32* srcCurrentPixel = srcPixels + row * blockW + col;
                *dstCurrentPixel = *srcCurrentPixel;
            }
        }

        SDL_FreeSurface(srcSurface);
    }

    createGlTexture(dstSurface);
    SDL_FreeSurface(dstSurface);
}

void MapTexture::createGlTexture(SDL_Surface *surface) {
    GLint nOfColors = 4;
    GLenum textureFormat = GL_BGRA;

    // Have OpenGL generate a texture object handle for us
    glGenTextures(1, &texture);

    // Bind the texture object
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set the texture's stretching properties
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Edit the texture object's image data using the information SDL_Surface gives us
    glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
            textureFormat, GL_UNSIGNED_BYTE, surface->pixels);
}

void MapTexture::glBind() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, this->texture);
}

void MapTexture::glUnbind() {
    glDisable(GL_TEXTURE_2D);
}

void MapTexture::renderBlock(int x, int y, int w, int h, int textureNumber) {
    glBegin(GL_QUADS);
    GLfloat textureWidth = 1.0 / count;
    glTexCoord2f(textureNumber * textureWidth, 0);
    glVertex2i(x, y);
    glTexCoord2f((textureNumber + 1) * textureWidth, 0);
    glVertex2i(x + w, y);
    glTexCoord2f((textureNumber + 1) * textureWidth, 1);
    glVertex2i(x + w, y + h);
    glTexCoord2f(textureNumber * textureWidth, 1);
    glVertex2i(x, y + h);
    glEnd();
}
