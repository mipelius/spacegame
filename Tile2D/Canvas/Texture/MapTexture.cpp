// MIT License
//
// This file is part of SpaceGame.
// Copyright (c) 2014-2018 Miika Pelkonen
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


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

int MapTexture::addTexture(std::string filename, float opacity) {
	SDL_Surface *surface;    // This surface will tell us the details of the image
	GLenum texture_format = GL_NONE;
	GLint nOfColors;
    
	surface = IMG_Load(filename.data());

    if (!surface) {
        std::fprintf(stderr, "SDL could not load %s: %s\n", filename.data(), SDL_GetError());
        SDL_Quit();
        exit(1);
    }
    if (surface->w != blockW_ || surface->h != blockH_) {
        std::fprintf(stderr, "Image has invalid size (%s)\n", filename.data());
		SDL_Quit();
    	exit(1);
    }

	nOfColors = surface->format->BytesPerPixel;

	if (nOfColors == 4)     // contains an alpha channel
	{
		if (surface->format->Rmask == 0x000000ff)
			texture_format = GL_RGBA;
		else
			texture_format = GL_BGRA;
	}
	else if (nOfColors == 3)     // no alpha channel
	{
		if (surface->format->Rmask == 0x000000ff)
			texture_format = GL_RGB;
		else
			texture_format = GL_BGR;
	}

    // Bind the texture object
    glBindTexture(GL_TEXTURE_2D, glTextureId_);

    // Update texture
    int offsetX = (blockInternalSize_ * count) % GL_TEXTURE_SIZE;
    int offsetY = (blockInternalSize_ * count) / GL_TEXTURE_SIZE;
	
    glTexSubImage2D(GL_TEXTURE_2D, 0, offsetX, offsetY, blockW_, blockH_, texture_format, GL_UNSIGNED_BYTE, surface->pixels);

	SDL_FreeSurface(surface);

    count++;

    return count - 1;
}