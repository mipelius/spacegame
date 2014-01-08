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

#include "Texture.h"

Texture::Texture(std::string filename) {
    SDL_Surface *surface;    // This surface will tell us the details of the image
    GLenum texture_format;
    GLint nOfColors;

    if ((surface = SDL_LoadBMP(filename.data()))) {

        // Check that the image's width is a power of 2
        if ((surface->w & (surface->w - 1)) != 0) {
            fprintf(stderr, "warning: %s 's width is not a power of 2\\n", filename.data());
        }

        // Also check if the height is a power of 2
        if ((surface->h & (surface->h - 1)) != 0) {
            fprintf(stderr, "warning: %s 's heigth is not a power of 2\\n", filename.data());
        }

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
        glGenTextures(1, &texture);

        // Bind the texture object
        glBindTexture(GL_TEXTURE_2D, texture);

        // Set the texture's stretching properties
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Edit the texture object's image data using the information SDL_Surface gives us
        glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
                texture_format, GL_UNSIGNED_BYTE, surface->pixels);
    }
    else {
        printf("SDL could not load %s: %s\n", filename.data(), SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    // Free the SDL_Surface only if it was successfully created
    if (surface) {
        SDL_FreeSurface(surface);
    }
}

void Texture::glBind() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, this->texture);
}

void Texture::glUnbind() {
    glDisable(GL_TEXTURE_2D);
}
