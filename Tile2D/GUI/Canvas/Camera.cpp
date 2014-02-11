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
#include "Body.h"
#include "PhysicsWorld.h"

Camera::Camera() :
    location(   new Location        (Point(0, 0))       ),
    areaRect(   new Property<Rect>  (Rect(0, 0, 0, 0))  ),
    boundsRect( new Property<Rect>  (Rect(0, 0, 0, 0))  ) {

    ((Location*)location)->areaRect = this->areaRect;
    ((Location*)location)->boundsRect = this->boundsRect;
}

Camera::~Camera() {
    delete location;
    delete areaRect;
    delete boundsRect;
}

Camera::Location::Location(Point const &initialValue) : Property(initialValue) { }

void Camera::Location::setActual(Point value) {
    value_ = value;

    double x = value.x - areaRect->get().getWidth() / 2;
    double y = value.y - areaRect->get().getHeight() / 2;
    double w = areaRect->get().getWidth();
    double h = areaRect->get().getHeight();

    double x1 = boundsRect->get().x1;
    double y1 = boundsRect->get().y1;
    double x2 = boundsRect->get().x2;
    double y2 = boundsRect->get().y2;

    if (x < x1)     x = x1;
    if (x + w > x2) x = x2 - w;
    if (y < y1)     y = y1;
    if (y + h > y2) y = y2 - h;

    areaRect->set(
            Rect(
                    x,
                    y,
                    x + w,
                    y + h
            )
    );
}