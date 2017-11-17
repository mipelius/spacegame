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

#include "Camera.h"

Camera::Camera() :
    // properties

    position(       Property<Vec>    (this, getPosition_, setPosition_   )),
    areaRect(       Property<Rect>   (&areaRect_                         )),
    boundsRect(     Property<Rect>   (&boundsRect_                       )),

    // member objects
    areaRect_(      Rect(0, 0, 0, 0)    ),
    boundsRect_(    Rect(0, 0, 0, 0)    )
{

}


void Camera::zoom(double amount) {
    double ratio = areaRect_.getHeight() / areaRect_.getWidth();

    areaRect_.x1 += amount;
    areaRect_.y1 += amount * ratio;
    areaRect_.x2 -= amount;
    areaRect_.y2 -= amount * ratio;
    areaRect.updateDependentProperties();
}

Vec Camera::getPosition_(void *owner) {
    Rect rect = ((Camera*)owner)->areaRect.get();

    double x = rect.x1 + rect.getWidth() / 2;
    double y = rect.y1 + rect.getHeight() / 2;

    return Vec(x, y);
}

void Camera::setPosition_(void *owner, const Vec &value) {
    Rect areaRect = ((Camera*)owner)->areaRect.get();
    Rect boundsRect = ((Camera*)owner)->boundsRect.get();

    double x = value.x - areaRect.getWidth() / 2;
    double y = value.y - areaRect.getHeight() / 2;
    double w = areaRect.getWidth();
    double h = areaRect.getHeight();

    double x1 = boundsRect.x1;
    double y1 = boundsRect.y1;
    double x2 = boundsRect.x2;
    double y2 = boundsRect.y2;

    if (x < x1)     x = x1;
    if (x + w > x2) x = x2 - w;
    if (y < y1)     y = y1;
    if (y + h > y2) y = y2 - h;

    ((Camera*)owner)->areaRect.set(
            Rect(
                    x,
                    y,
                    x + w,
                    y + h
            )
    );
}