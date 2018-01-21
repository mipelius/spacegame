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

    position(       Property<Vecf>    (this, getPosition_, setPosition_   )),
    areaRect(       Property<Rect>   (&areaRect_                         )),

    // member objects
    areaRect_(      Rect(0, 0, 0, 0)    )
{

}


void Camera::zoom(float amount) {
    float ratio = areaRect_.getHeight() / areaRect_.getWidth();

    areaRect_.x1 += amount;
    areaRect_.y1 += amount * ratio;
    areaRect_.x2 -= amount;
    areaRect_.y2 -= amount * ratio;
    areaRect.updateDependentProperties();
}

Vecf Camera::getPosition_(void *owner) {
    Rect rect = ((Camera*)owner)->areaRect.get();

    float x = rect.x1 + rect.getWidth() / 2;
    float y = rect.y1 + rect.getHeight() / 2;

    return Vecf(x, y);
}

void Camera::setPosition_(void *owner, const Vecf &value) {
    Rect areaRect = ((Camera*)owner)->areaRect.get();

    float x = value.x - areaRect.getWidth() / 2;
    float y = value.y - areaRect.getHeight() / 2;
    float w = areaRect.getWidth();
    float h = areaRect.getHeight();

    ((Camera*)owner)->areaRect.set(
            Rect(
                    x,
                    y,
                    x + w,
                    y + h
            )
    );
}