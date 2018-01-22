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
}

// getters and setters

Vecf Camera::getPosition() {
    float x = areaRect_.x1 + areaRect_.getWidth() / 2;
    float y = areaRect_.y1 + areaRect_.getHeight() / 2;

    return Vecf(x, y);
}

void Camera::setPosition(const Vecf &value) {
    float w = areaRect_.getWidth();
    float h = areaRect_.getHeight();

    areaRect_.x1 = value.x - w / 2;
    areaRect_.y1 = value.y - h / 2;
    areaRect_.x2 = areaRect_.x1 + w;
    areaRect_.y2 = areaRect_.y1 + h;
}

const Rect &Camera::getAreaRect() const {
    return areaRect_;
}

void Camera::setAreaRect(const Rect &areaRect) {
    areaRect_ = areaRect;
}
