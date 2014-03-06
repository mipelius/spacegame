// This file is part of SpaceGame.
// Copyright (C) 2014 Miika Pelkonen
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
#include "LightMask.h"
#include "Camera.h"
#include "ILight.h"


LightMask::LightMask(int w, int h) :
    ambientLight    (   new SimpleProperty<double>( &ambientLight_ )    ),
    ambientLight_   (   1.0 )
{
    w_ = w;
    h_ = h;
    initialize();
}

LightMask::~LightMask() {
    delete ambientLight;
}

void LightMask::draw(Canvas *canvas) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, glTextureId_);

    clearPixels();

    for (std::list<ILight*>::iterator i = lights.begin(); i != lights.end(); i++) {
        (*i)->draw(canvas, this);
    }

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w_, h_, GL_RGBA, GL_UNSIGNED_BYTE, pixels_);

    glColor4d(1, 1, 1, 1);

    glBegin(GL_QUADS);
    glTexCoord2d(0, 0);
    glVertex2d(0, 0);
    glTexCoord2d(1, 0);
    glVertex2d(w_, 0);
    glTexCoord2d(1, 1);
    glVertex2d(w_, h_);
    glTexCoord2d(0, 1);
    glVertex2d(0, h_);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    glDisable(GL_BLEND);
}


void LightMask::clearPixels() {
    Uint8 ambientLight = getNormalizedAmbientLightByte();

    for (int i = 0; i < w_ * h_; i++) {
        pixels_[i] = (0x000000FF - ambientLight) << 24;
    }
}

void LightMask::initialize() {
    GLenum texture_format = GL_BGRA;
    GLint nOfColors = 4;

    pixels_ = new Uint32[w_ * h_];

    clearPixels();

    // Have OpenGL generate a texture object handle for us
    glGenTextures(1, &glTextureId_);

    // Bind the texture object
    glBindTexture(GL_TEXTURE_2D, glTextureId_);

    // Set the texture's stretching properties
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Edit the texture object's image data using the information SDL_Surface gives us
    glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, w_, h_, 0,
            texture_format, GL_UNSIGNED_BYTE, pixels_);
}

void LightMask::add(ILight *light) {
    lights.push_back(light);
}

Uint8 LightMask::getNormalizedAmbientLightByte() {
    double value;

    if (ambientLight_ < 0) {
        value = 0;
    }
    else if (ambientLight_ > 1.0) {
        value = 1.0;
    }
    else {
        value = ambientLight_;
    }

    return (Uint8)(value * 255);
}

void LightMask::addAlpha(int x, int y, double value) {
    if (x < 0 || x >= w_) return;
    if (y < 0 || y >= h_) return;

    Uint8 oldValue = (Uint8)(pixels_[y * w_ + x] >> 24);
    Uint8 subtraction = (Uint8)(value * oldValue);
    pixels_[y * w_ + x] = (oldValue - subtraction) << 24;
}
