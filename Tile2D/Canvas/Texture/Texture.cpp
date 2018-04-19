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
#include "Texture.h"
#include "Tile2D.h"

Texture::Texture(
        std::string filename,
        GLint       minFilter,
        GLint       magFilter
) {
    SDL_Surface *surface;    // This surface will tell us the details of the image
    GLenum texture_format = GL_NONE;
    GLint nOfColors;

	surface = IMG_Load((Tile2D::getResourcePath() + filename).data());
    if (!surface)
	{
		std::string error = "Could not load texture file: ";
		error=error.append(SDL_GetError());
		throw std::exception();
	}

	this->w_ = surface->w;
    this->h_ = surface->h;

#ifdef DEBUG
    // Check that the image's width is a power of 2
    if ((surface->w & (surface->w - 1)) != 0) {
        fprintf(stderr, "warning: %s 's width is not a power of 2\n", filename.data());
    }

    // Also check if the height is a power of 2
    if ((surface->h & (surface->h - 1)) != 0) {
        fprintf(stderr, "warning: %s 's heigth is not a power of 2\n", filename.data());
    }
#endif

    // get the number of channels in the SDL surface
    nOfColors = surface->format->BytesPerPixel;
    if (nOfColors == 4)     // contains an alpha channel
    {
        if (surface->format->Rmask == 0x000000ff)
            texture_format = GL_RGBA;
        else
            texture_format = GL_BGRA;
    } else if (nOfColors == 3)     // no alpha channel
    {
        if (surface->format->Rmask == 0x000000ff)
            texture_format = GL_RGB;
        else
            texture_format = GL_BGR;
    } else {
        printf("warning: the image is not truecolor..  this will probably break\n");
        // this error should not go unhandled
    }

    // Have OpenGL generate a texture object handle for us
    glGenTextures(1, &texture_);

    // Bind the texture object
    glBindTexture(GL_TEXTURE_2D, texture_);

    // Set the texture's stretching properties
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

    // Edit the texture object's image data using the information SDL_Surface gives us
    glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
            texture_format, GL_UNSIGNED_BYTE, surface->pixels);

    // Free the SDL_Surface only if it was successfully created
    SDL_FreeSurface(surface);

}

void Texture::glBind() {
    glBindTexture(GL_TEXTURE_2D, texture_);
}

int Texture::getW() {
    return this->w_;
}

int Texture::getH() {
    return this->h_;
}