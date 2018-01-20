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

void MapTexture::createGlTexture() {
    GLint nOfColors = 4;
    GLenum textureFormat = GL_BGRA;

    // Have OpenGL generate a texture object handle for us
    glGenTextures(1, &glTextureId_);

    // Bind the texture object
    glBindTexture(GL_TEXTURE_2D, glTextureId_);

    // Set the texture's stretching properties
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


    // create empty pixels
    Uint32* pixels = new Uint32[GL_TEXTURE_SIZE * GL_TEXTURE_SIZE];
    memset(pixels, '\0', sizeof(Uint32) * GL_TEXTURE_SIZE * GL_TEXTURE_SIZE);

    // Edit the texture object's image data using the information SDL_Surface gives us
    glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, GL_TEXTURE_SIZE, GL_TEXTURE_SIZE, 0,
            textureFormat, GL_UNSIGNED_BYTE, pixels);

    delete[] pixels;
}

void MapTexture::glBind() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, this->glTextureId_);
}

void MapTexture::glUnbind() {
    glDisable(GL_TEXTURE_2D);
}

void MapTexture::glVertices(float x, float y, float w, float h, int textureNumber, int cornerRounding) {
    float textureW = blockW_ / (float)GL_TEXTURE_SIZE;
    float textureH = blockH_ / (float)GL_TEXTURE_SIZE;

    float texX1 = ((textureNumber * blockInternalSize_) % GL_TEXTURE_SIZE) / (float)GL_TEXTURE_SIZE;
    float texY1 = ((textureNumber * blockInternalSize_) / GL_TEXTURE_SIZE) / (float)GL_TEXTURE_SIZE;
    float texX2 = texX1 + textureW;
    float texY2 = texY1 + textureH;

    float margin = 0.00000001;

    texX1 += margin;
    texY1 += margin;
    texX2 -= margin;
    texY2 -= margin;

    glTexCoord2f((GLfloat)texX1, (GLfloat)texY1);    glVertex2f((GLfloat) x     , (GLfloat) y     );
    glTexCoord2f((GLfloat)texX2, (GLfloat)texY1);    glVertex2f((GLfloat)(x + w), (GLfloat) y     );
    glTexCoord2f((GLfloat)texX2, (GLfloat)texY2);    glVertex2f((GLfloat)(x + w), (GLfloat)(y + h));
    glTexCoord2f((GLfloat)texX1, (GLfloat)texY2);    glVertex2f((GLfloat) x     , (GLfloat)(y + h));
}

MapTexture::MapTexture(int blockW, int blockH) {
    count = 0;

    blockW_ = blockW;
    blockH_ = blockH;

    int temp = (blockW > blockH ? blockW : blockH);

    int blockInternalSize;

    for (int i=0; i<8; i++) {
        blockInternalSize = (int)pow(2, i);
        if (blockInternalSize > temp) break;
    }

    blockInternalSize_ = blockInternalSize;

    createGlTexture();
}

int MapTexture::addTexture(std::string filename) {
    SDL_Surface* surface = IMG_Load(filename.data());

    if (!surface) {
        std::fprintf(stderr, "SDL could not load %s: %s\n", filename.data(), SDL_GetError());
        SDL_Quit();
        exit(1);
    }
    if (surface->w != blockW_ || surface->h != blockH_) {
        std::fprintf(stderr, "Image has invalid size (%s)\n", filename.data());
        exit(1);
    }

    // Bind the texture object
    glBindTexture(GL_TEXTURE_2D, glTextureId_);

    // Update texture
    int offsetX = (blockInternalSize_ * count) % GL_TEXTURE_SIZE;
    int offsetY = (blockInternalSize_ * count) / GL_TEXTURE_SIZE;

    glTexSubImage2D(GL_TEXTURE_2D, 0, offsetX, offsetY, blockW_, blockH_, GL_BGRA, GL_UNSIGNED_BYTE, surface->pixels);

    count++;

    return count - 1;
}
