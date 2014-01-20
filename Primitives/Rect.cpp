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

#include "Rect.h"

Rect::Rect(Point firstPoint, Point secondPoint): firstPoint(firstPoint), secondPoint(secondPoint) {
    // if (firstPoint.x == secondPoint.x || firstPoint.y == secondPoint.y) throw exception because not rectangle?

    // change the locations so that the first point is the top left corner and the second point is the bottom right corner
    if (firstPoint.x > secondPoint.x) {
        this->firstPoint.x = secondPoint.x;
        this->secondPoint.x = firstPoint.x;
    }

    if (firstPoint.y > secondPoint.y) {
        this->firstPoint.y = secondPoint.y;
        this->secondPoint.y = firstPoint.y;
    }
}

Point Rect::getTopLeftCorner() {
    return firstPoint;
}

Point Rect::getTopRightCorner() {
    return Point(secondPoint.x, firstPoint.y);
}

Point Rect::getBottomLeftCorner() {
    return Point(firstPoint.x, secondPoint.y);
}

Point Rect::getBottomRightCorner() {
    return secondPoint;
}

double Rect::getWidth() {
    return secondPoint.x - firstPoint.x;
}

double Rect::getHeight() {
    return secondPoint.y - firstPoint.y;
}

bool Rect::intersectsWith(Rect otherRectangle) {
    double rectAx1 = this->getTopLeftCorner().x;
    double rectAy1 = this->getTopLeftCorner().y;
    double rectAx2 = this->getBottomRightCorner().x;
    double rectAy2 = this->getBottomRightCorner().y;

    double rectBx1 = otherRectangle.getTopLeftCorner().x;
    double rectBy1 = otherRectangle.getTopLeftCorner().y;
    double rectBx2 = otherRectangle.getBottomRightCorner().x;
    double rectBy2 = otherRectangle.getBottomRightCorner().y;

    return rectAx1 <= rectBx2 && rectAx2 >= rectBx1 && rectAy1 <= rectBy2 && rectAy2 >= rectBy1;
}

Point Rect::getFirstPoint() {
    return firstPoint;
}

Point Rect::getSecondPoint() {
    return secondPoint;
}

bool Rect::intersectsWithLine(double x1, double y1, double x2, double y2) {
    // if one of the points is inside the rectangle
    if (
            (x1 >= firstPoint.x && x1 <= secondPoint.x) && (y1 >= firstPoint.y && y1 <= secondPoint.y)
            ) return true;

    if (
            (x2 >= firstPoint.x && x2 <= secondPoint.x) && (y2 >= firstPoint.y && y2 <= secondPoint.y)
            ) return true;

    double slope = (y2 - y1) / (x2 - x1);

    // top line

    double y = firstPoint.y;
    double x = (y + slope * x1 - y1) / slope;
    if (
            x >= firstPoint.x && x <= secondPoint.x &&
                    ((y1 >= y && y2 <= y) ||
                    (y1 <= y && y2 >= y))
            ) return true;

    // bottom line

    y = secondPoint.y;
    x = (y + slope * x1 - y1) / slope;
    if (
            x >= firstPoint.x && x <= secondPoint.x &&
                    ((y1 >= y && y2 <= y) ||
                    (y1 <= y && y2 >= y))
            ) return true;

    // left line

    x = firstPoint.x;
    y = slope * x - slope * x1 + y1;
    if (
            y >= firstPoint.y && y <= secondPoint.y &&
                    ((x1 >= x && x2 <= x) ||
                    (x1 <= x && x2 >= x))
            ) return true;

    // right line

    x = secondPoint.x;
    y = slope * x - slope * x1 + y1;
    if (y >= firstPoint.y && y <= secondPoint.y &&
            ((x1 >= x && x2 <= x) ||
            (x1 <= x && x2 >= x))
            ) return true;

    return false;
}
