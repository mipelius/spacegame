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

Tile2DComponent *PointLight::clone() {
    return new PointLight(*this);
}

void PointLight::deserialize(const json::Object &jsonObject) {
    if (jsonObject.HasKey("radius")) {
        radius_ = jsonObject["radius"].ToFloat();
    }
    if (jsonObject.HasKey("intensity")) {
        intensity_ = jsonObject["intensity"].ToFloat();
    }
}
