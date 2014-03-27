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
#include "ShadowMask.h"
#include "Canvas.h"
#include "ILight.h"
#include "WorldMap.h"
#include "ShadowMap.h"
#include "PointLight.h"
#include "LightMap.h"

ShadowMask::ShadowMask(double w, double h, WorldMap * map) :

ambientLight    (   new SimpleProperty<double>( &ambientLight_ )    ),
ambientLight_   (   1.0 )

{
    w_ = w;
    h_ = h;

    shadowMap_ = new ShadowMap(map, this);

    initialize();
}

ShadowMask::~ShadowMask() {
    delete shadowMap_;
}

void ShadowMask::update(Canvas *canvas) {
    // turn projection upside down

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w_, 0, h_, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    // update dynamic light lightMaps

    for (std::list<PointLight*>::iterator i = dynamicLights_.begin(); i != dynamicLights_.end(); i++) {

    }

    // draw shadow map to texture

    glColor4d(0, 0, 0, 1.0);
    glRectd(0, 0, w_, h_);

    shadowMap_->draw(canvas);

    glBindTexture(GL_TEXTURE_2D, glTextureId_);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, (int)w_, (int)h_);

    // draw lights

    glColor4d(0, 0, 0, 1.0);
    glRectd(0, 0, w_, h_);

    glEnable(GL_BLEND);
    glBlendFunc(GL_DST_ALPHA, GL_ZERO);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, PointLight::glTextureId_);

    glColor4d(0.0, 0.0, 0.0, 1.0);
    glBegin(GL_QUADS);


    for (std::list<PointLight*>::iterator i = staticLights_.begin(); i != staticLights_.end(); i++) {
        (*i)->draw(canvas);
    }

    for (std::list<PointLight*>::iterator i = dynamicLights_.begin(); i != dynamicLights_.end(); i++) {
        (*i)->draw(canvas);
    }

    glEnd();

    glDisable(GL_BLEND);

    // blend it with map shadows

    glEnable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, glTextureId_);

    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

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

    glDisable(GL_BLEND);

    // update mask texture

    glDisable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, glTextureId_);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, (int)w_, (int)h_);
    glBindTexture(GL_TEXTURE_2D, NULL);

    // set "normal" projection

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w_, h_, 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
}

void ShadowMask::draw(Canvas *canvas) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, glTextureId_);

    glColor4d(1, 1, 1, 1.0 - ambientLight_);

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

void ShadowMask::addStaticLight(PointLight *light) {
    staticLights_.push_back(light);
    shadowMap_->updateStaticLightMap(light);
}

void ShadowMask::addDynamicLight(PointLight *light) {
    dynamicLights_.push_back(light);
}

void ShadowMask::initialize() {
    GLenum texture_format = GL_BGRA;
    GLint nOfColors = 4;

    Uint32* pixels = new Uint32[(int)w_ * (int)h_];

    // Have OpenGL generate a texture object handle for us
    glGenTextures(1, &glTextureId_);

    // Bind the texture object
    glBindTexture(GL_TEXTURE_2D, glTextureId_);

    // Set the texture's stretching properties
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Edit the texture object's image data using the information SDL_Surface gives us
    glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, (int)w_, (int)h_, 0,
            texture_format, GL_UNSIGNED_BYTE, pixels);

    delete[] pixels;
}
