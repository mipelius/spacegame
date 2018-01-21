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
    position(   Property<Vecf>  (&position_)    ),
    size(       Property<float>  (&size_   )     ),

    position_(  Vecf(0, 0) ),
    size_(      1.0          )
{

}

void Plot::draw(const Canvas &canvas) {
    Rect cameraRect = canvas.getCamera()->areaRect.get();
    Rect renderingAreaRect = canvas.getRenderingAreaRect();

    float pixelWidth = cameraRect.getWidth() / renderingAreaRect.getWidth();
    float pixelHeight = cameraRect.getHeight() / renderingAreaRect.getHeight();

    float addition;

    if (size_ <= 1.0) {
        addition = 0;
    }
    else {
        addition = size_ / 2;
    }

    float x1 = position_.x - addition * pixelWidth;
    float y1 = position_.y - addition * pixelHeight;
    float x2 = x1 + pixelWidth + addition * pixelWidth * 2;
    float y2 = y1 + pixelHeight + addition * pixelHeight * 2;

    canvas.glColor(1, 1, 1);

    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}
