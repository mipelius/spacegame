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
#include "Plot.h"
#include "Canvas.h"
#include "Camera.h"

Plot::Plot() :
    location(   Property<Point>   (&location_)    ),
    size(       Property<double>  (&size_   )     ),

    location_(  Point(0, 0) ),
    size_(      1.0         )
{

}

void Plot::draw(Canvas *canvas) {
    Rect cameraRect = canvas->getCamera().areaRect.get();
    Rect renderingAreaRect = canvas->getRenderingAreaRect();

    double pixelWidth = cameraRect.getWidth() / renderingAreaRect.getWidth();
    double pixelHeight = cameraRect.getHeight() / renderingAreaRect.getHeight();

    double addition;

    if (size_ <= 1.0) {
        addition = 0;
    }
    else {
        addition = size_ / 2;
    }

    double x1 = location_.x - addition * pixelWidth;
    double y1 = location_.y - addition * pixelHeight;
    double x2 = x1 + pixelWidth + addition * pixelWidth * 2;
    double y2 = y1 + pixelHeight + addition * pixelHeight * 2;

    canvas->glColor(1, 1, 1);

    glBegin(GL_QUADS);
    glVertex2d(x1, y1);
    glVertex2d(x2, y1);
    glVertex2d(x2, y2);
    glVertex2d(x1, y2);
    glEnd();
}
