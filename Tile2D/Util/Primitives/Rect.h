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

#ifndef __Rect_H_
#define __Rect_H_

#include "Vector.h"

class Rect {
public:
    double x1;
    double y1;
    double x2;
    double y2;

    Rect(double x1, double y1, double x2, double y2);

    inline void setPosition(const Vector &point);

    inline double getWidth() const;
    inline double getHeight() const;

    inline bool hasPointInside(const Vector &point) const;

    inline bool intersectsWith(const Rect &otherRectangle) const;
    inline bool intersectsWithLine(const double &x1, const double &y1, const double &x2, const double &y2) const;

    inline void copy(Rect rect);
};

// inline functions

inline bool Rect::intersectsWith(const Rect &otherRectangle) const {
    const double& rectAx1 = x1;
    const double& rectAy1 = y2;
    const double& rectAx2 = x1;
    const double& rectAy2 = y2;

    const double& rectBx1 = otherRectangle.x1;
    const double& rectBy1 = otherRectangle.y1;
    const double& rectBx2 = otherRectangle.x2;
    const double& rectBy2 = otherRectangle.y2;

    return rectAx1 <= rectBx2 && rectAx2 >= rectBx1 && rectAy1 <= rectBy2 && rectAy2 >= rectBy1;
}

inline bool Rect::intersectsWithLine(const double &x1, const double &y1, const double &x2, const double &y2) const {
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
    if (
            y >= this->y1 && y <= this->y2 &&
            ((x1 >= x && x2 <= x) ||
             (x1 <= x && x2 >= x))
            ) return true;

    return false;
}

inline void Rect::setPosition(const Vector &point) {
    double w = this->getWidth();
    double h = this->getHeight();
    x1 = point.x;
    y1 = point.y;
    x2 = point.x + w;
    y2 = point.y + h;
}

inline double Rect::getWidth() const {
    return x2 - x1;
}

inline double Rect::getHeight() const {
    return y2 - y1;
}

inline void Rect::copy(Rect rect) {
    this->x1 = rect.x1;
    this->y1 = rect.y1;
    this->x2 = rect.x2;
    this->y2 = rect.y2;
}

inline bool Rect::hasPointInside(const Vector &point) const {
    return (point.x >= this->x1 && point.x <= this->x2) &&
           (point.y >= this->y1 && point.y <= this->y2);
}

#endif //__Rect_H_
