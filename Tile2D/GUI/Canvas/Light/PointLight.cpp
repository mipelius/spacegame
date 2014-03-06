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

#include "PointLight.h"
#include "SimpleProperty.h"
#include "Canvas.h"
#include "Camera.h"

#include "LightMask.h"

PointLight::PointLight(Point location, double radius) :
    location    (   new SimpleProperty<Point>   (&location_ )  ),
    radius      (   new SimpleProperty<double>  (&radius_   )  ),

    location_   (   location    ),
    radius_     (   radius      )
{
    int w = (int)(radius_ * 2);
    int h = (int)(radius_ * 2);

    alphaValues = new double[w * h];

    double alphaValue;

    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            alphaValue = sin(i * M_PI / w) * sin(j * M_PI / w);
            alphaValue = pow(alphaValue, 4);
            alphaValues[i + j * w] = alphaValue;
        }
    }
}



void PointLight::draw(Canvas *canvas, LightMask *lightMask) {
    Rect rect = canvas->getCamera()->areaRect->get();

    int offsetX = (int)(location_.x - rect.x1 - radius_);
    int offsetY = (int)(location_.y - rect.y1 - radius_);

    int w = (int)(radius_ * 2);
    int h = (int)(radius_ * 2);

    double alphaValue;

    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            alphaValue = alphaValues[i + j * w];
            lightMask->addAlpha(offsetX + i, offsetY + j, alphaValue);
        }
    }
}

PointLight::~PointLight() {
    delete[] alphaValues;
}
