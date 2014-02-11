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
            std::fprintf(stderr, "SDL could not load %s: %s\n", fileNames[i].data(), SDL_GetError());
            SDL_Quit();
            exit(1);
        }
        if (srcSurface->w != blockW || srcSurface->h != blockH) {
            std::fprintf(stderr, "Image has invalid size (%s)\n", fileNames[i].data());
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
