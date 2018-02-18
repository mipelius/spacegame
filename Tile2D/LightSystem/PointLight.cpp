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

#include "Tile2D.h"
#include "precompile.h"
#include "PointLight.h"
#include "Canvas.h"
#include "Camera.h"
#include "Tile2DMath.h"
#include "LightSystem.h"

GLuint PointLight::glTextureId_ = 0;

PointLight::PointLight() :
    radius_     (   0         ),
    intensity_  (   1.0       )
{
    if (glTextureId_ == 0) {
        createLightTexture();
    }
}

void PointLight::draw(const Canvas &canvas) {
    const Rect& rect = canvas.getCamera()->getAreaRect();

    float x = transform()->getPosition().x - rect.x1 - radius_;
    float y = transform()->getPosition().y - rect.y1 - radius_;
    float w = radius_ * 2;
    float h = radius_ * 2;

    float margin = 0.0f;

    glColor4f(0.0f, 0.0f, 0.0f, (GLfloat)intensity_);

    glTexCoord2f(margin, margin);
    glVertex2f(x, y);
    glTexCoord2f(1.0f - margin, margin);
    glVertex2f(x + w, y);
    glTexCoord2f(1.0f - margin, 1.0f - margin);
    glVertex2f(x + w, y + h);
    glTexCoord2f(margin, 1.0f - margin);
    glVertex2f(x, y + h);
}

void PointLight::createLightTexture() {
	Uint8 alphaValue = 0;
	int tempAlpha = 0;
    double tempAlphaDouble = 0.0;

    Uint32* lightPixels = new Uint32[TEXTURE_SIZE * TEXTURE_SIZE];
	int radius = TEXTURE_SIZE / 2;

    for (int i = 0; i < TEXTURE_SIZE; i++) {
        for (int j = 0; j < TEXTURE_SIZE; j++) {
			Vecd vec = { (double)(radius - i), (double)(radius - j) };
        	
			double distance = vec.length();

            tempAlphaDouble = distance / (radius);

            tempAlpha = (int)(tempAlphaDouble * 255);
			Mathi::clamp(tempAlpha, 0, 255);

			alphaValue = (Uint8)tempAlpha;

            lightPixels[j * TEXTURE_SIZE + i] = (0x000000FF - alphaValue) << 24;
        }
    }

    GLenum texture_format = GL_BGRA;
    GLint nOfColors = 4;

    // Have OpenGL generate a texture object handle for us
    glGenTextures(1, &glTextureId_);

    // Bind the texture object
    glBindTexture(GL_TEXTURE_2D, glTextureId_);

    // Set the texture's stretching properties
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Edit the texture object's image data using the information SDL_Surface gives us
    glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, TEXTURE_SIZE, TEXTURE_SIZE, 1,
            texture_format, GL_UNSIGNED_BYTE, lightPixels);

    delete[] lightPixels;
}

void PointLight::init() {
    Tile2D::lightSystem().addLight(this);
}

void PointLight::onDestroy() {
    Tile2D::lightSystem().removeLight(this);
}

// getters and setters

float PointLight::getRadius() const {
    return radius_;
}

void PointLight::setRadius(float radius) {
    radius_ = radius;
}

float PointLight::getIntensity() const {
    return intensity_;
}

void PointLight::setIntensity(float intensity) {
    intensity_ = intensity;
}
