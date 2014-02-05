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

#include "precompile.h"
#include "Camera.h"
#include "GameEntity.h"
#include "GameWorld.h"

Camera::Camera(Rect rect, Rect boundsRect) : rect_(rect), boundsRect_(boundsRect) {

}

Point Camera::getLocation() {
    return Point(rect_.x1, rect_.y1);
}

void Camera::setLocation(Point point) {
    double x = point.x;
    double y = point.y;
    double w = rect_.getWidth();
    double h = rect_.getHeight();

    double x1 = boundsRect_.x1;
    double y1 = boundsRect_.y1;
    double x2 = boundsRect_.x2;
    double y2 = boundsRect_.y2;

    if (x < x1)     x = x1;
    if (x + w > x2) x = x2 - w;
    if (y < y1)     y = y1;
    if (y + h > y2) y = y2 - h;

    rect_.setLocation(Point(x, y));
}

void Camera::setCenteredLocation(Point point) {
    point.x -= rect_.getWidth() / 2;
    point.y -= rect_.getHeight() / 2;
    setLocation(point);
}

Rect Camera::getBounds() {
    return boundsRect_;
}

Rect Camera::getRect() {
    return rect_;
}

Point Camera::getCenteredLocation() {
    return Point(rect_.x1 + rect_.getWidth() / 2, rect_.y1 + rect_.getHeight() / 2);
}
