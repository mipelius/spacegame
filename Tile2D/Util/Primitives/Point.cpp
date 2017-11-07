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
#include "Point.h"
#include "Rect.h"

Point::Point(double x, double y) {
    this->x = x;
    this->y = y;
}

Point::~Point() {
}


Point Point::operator +(const Vector &vector) {
    return Point(x + vector.x, y + vector.y);
}

Point Point::operator -(const Vector &vector) {
    return Point(x - vector.x, y - vector.y);
}

double Point::distance(const Point &otherPoint) const {
    return sqrt(
            (otherPoint.x - this->x) * (otherPoint.x - this->x) +
            (otherPoint.y - this->y) * (otherPoint.y - this->y)
    );
}

bool Point::isIn(const Rect& rect) {
    return
            x >= rect.x1 &&
            x <= rect.x2 &&
            y >= rect.y1 &&
            y <= rect.y2;
}

bool Point::equals(Point otherPoint) {
    return this->x == otherPoint.x && this->y == otherPoint.y;
}
