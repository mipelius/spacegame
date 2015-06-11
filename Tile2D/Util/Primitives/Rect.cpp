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
#include "Rect.h"

Rect::Rect(double x1, double y1, double x2, double y2) {
    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;
}

bool Rect::intersectsWith(Rect otherRectangle) {
    double rectAx1 = x1;
    double rectAy1 = y2;
    double rectAx2 = x1;
    double rectAy2 = y2;

    double rectBx1 = otherRectangle.x1;
    double rectBy1 = otherRectangle.y1;
    double rectBx2 = otherRectangle.x2;
    double rectBy2 = otherRectangle.y2;

    return rectAx1 <= rectBx2 && rectAx2 >= rectBx1 && rectAy1 <= rectBy2 && rectAy2 >= rectBy1;
}

bool Rect::intersectsWithLine(double x1, double y1, double x2, double y2) {
    // if one of the points is inside the rectangle
    if (
            (x1 >= this->x1 && x1 <= this->x2) && (y1 >= this->y1 && y1 <= this->y2)
            ) return true;

    if (
            (x2 >= this->x1 && x2 <= this->x2) && (y2 >= this->y1 && y2 <= this->y2)
            ) return true;

    double slope = (y2 - y1) / (x2 - x1);

    // top line

    double y = this->y1;
    double x = (y + slope * x1 - y1) / slope;
    if (
            x >= this->x1 && x <= this->x2 &&
                    ((y1 >= y && y2 <= y) ||
                    (y1 <= y && y2 >= y))
            ) return true;

    // bottom line

    y = this->y2;
    x = (y + slope * x1 - y1) / slope;
    if (
            x >= this->x1 && x <= this->x2 &&
                    ((y1 >= y && y2 <= y) ||
                    (y1 <= y && y2 >= y))
            ) return true;

    // left line

    x = this->x1;
    y = slope * x - slope * x1 + y1;
    if (
            y >= this->y1 && y <= this->y2 &&
                    ((x1 >= x && x2 <= x) ||
                    (x1 <= x && x2 >= x))
            ) return true;

    // right line

    x = this->x2;
    y = slope * x - slope * x1 + y1;
    if (y >= this->y1 && y <= this->y2 &&
            ((x1 >= x && x2 <= x) ||
            (x1 <= x && x2 >= x))
            ) return true;

    return false;
}

void Rect::setLocation(Point point) {
    double w = this->getWidth();
    double h = this->getHeight();
    x1 = point.x;
    y1 = point.y;
    x2 = point.x + w;
    y2 = point.y + h;
}

double Rect::getWidth() {
    return x2 - x1;
}

double Rect::getHeight() {
    return y2 - y1;
}

void Rect::copy(Rect rect) {
    this->x1 = rect.x1;
    this->y1 = rect.y1;
    this->x2 = rect.x2;
    this->y2 = rect.y2;
}

bool Rect::hasPointInside(Point point) {
    if (
            (point.x >= this->x1 && point.x <= this->x2) &&
            (point.y >= this->y1 && point.y <= this->y2)
    ) {
        return true;
    }
    else {
        return false;
    }
}
