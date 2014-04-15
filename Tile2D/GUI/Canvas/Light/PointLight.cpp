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
#include "PointLight.h"
#include "SimpleProperty.h"
#include "Canvas.h"
#include "Camera.h"
#include "WorldMap.h"
#include "PartialLightMap.h"

class PointLight::LocationProperty : public SimpleProperty<Point> {

public:
    LocationProperty(Point *actualData, PointLight* owner) : SimpleProperty(actualData) {
        owner_ = owner;
    }

    void setActual(Point value) {
        Point oldLocation = owner_->location_;

        SimpleProperty<Point>::setActual(value);

        Point newLocation = owner_->location_;

        owner_->movement->raise(PointLightMovedEventArgs(oldLocation, newLocation));
    }

private:

    PointLight* owner_;
};

GLuint PointLight::glTextureId_ = 0;

PointLight::PointLight(Point location, double radius) :
    location    (   new LocationProperty        (&location_, this   )  ),
    radius      (   new SimpleProperty<double>  (&radius_           )  ),
    intensity   (   new SimpleProperty<double>  (&intensity_        )  ),

    location_   (   location    ),
    radius_     (   radius      ),
    intensity_  (   1.0         ),

    movement    (   new Event<PointLight, PointLightMovedEventArgs>(this)   )
{
    if (glTextureId_ == 0) {
        createLightTexture();
    }

    int w = (int)(radius_ * 2 / 10);
    int h = (int)(radius_ * 2 / 10);
}

PointLight::~PointLight() {
    delete movement;

    delete location;
    delete radius;
}

void PointLight::draw(Canvas *canvas) {
    Rect rect = canvas->getCamera()->areaRect->get();

    double x = location_.x - rect.x1 - radius_;
    double y = location_.y - rect.y1 - radius_;
    double w = radius_ * 2;
    double h = radius_ * 2;

    double margin = 0.0;

    glColor4f(0.0, 0.0, 0.0, (GLfloat)intensity_);

    glTexCoord2f(margin, margin);
    glVertex2f(x, y);
    glTexCoord2f(1 - margin, margin);
    glVertex2f(x + w, y);
    glTexCoord2f(1 - margin, 1 - margin);
    glVertex2f(x + w, y + h);
    glTexCoord2f(margin, 1 - margin);
    glVertex2f(x, y + h);
}

void PointLight::createLightTexture() {
    std::cout << TEXTURE_SIZE;

    int w = TEXTURE_SIZE;
    int h = TEXTURE_SIZE;

    Uint8 alphaValue;
    double temp = 0.0;

    Uint32* lightPixels = new Uint32[w * h];

    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
//            temp = sin(i * M_PI / w) * sin(j * M_PI / w);
//            temp = temp * temp * temp * temp;

            double deltaX = w/2 - i;
            double deltaY = h/2 - j;

            double distance = sqrt(deltaX * deltaX + deltaY * deltaY);

            temp = distance / (w / 2);
            if (temp > 1.0) {
                temp = 1.0;
            }

            alphaValue = (Uint8)(temp * 255);
            lightPixels[j * w + i] = (0x000000FF - alphaValue) << 24;
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
    glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, w, h, 1,
            texture_format, GL_UNSIGNED_BYTE, lightPixels);

    delete[] lightPixels;
}

