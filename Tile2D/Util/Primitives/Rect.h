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

#include "Vec.h"

class Rect {
public:
    float x1;
    float y1;
    float x2;
    float y2;

    Rect(float x1, float y1, float x2, float y2);

    inline void setPosition(const Vecf &point);

    inline float getWidth() const;
    inline float getHeight() const;

    inline bool hasPointInside(const Vecf &point) const;

    inline bool intersectsWith(const Rect &otherRectangle) const;
    inline bool intersectsWithLine(const float &x1, const float &y1, const float &x2, const float &y2) const;

    inline void copy(Rect rect);
};

// inline functions

inline bool Rect::intersectsWith(const Rect &otherRectangle) const {
    const float& rectAx1 = x1;
    const float& rectAy1 = y2;
    const float& rectAx2 = x1;
    const float& rectAy2 = y2;

    const float& rectBx1 = otherRectangle.x1;
    const float& rectBy1 = otherRectangle.y1;
    const float& rectBx2 = otherRectangle.x2;
    const float& rectBy2 = otherRectangle.y2;

    return rectAx1 <= rectBx2 && rectAx2 >= rectBx1 && rectAy1 <= rectBy2 && rectAy2 >= rectBy1;
}

inline bool Rect::intersectsWithLine(const float &x1, const float &y1, const float &x2, const float &y2) const {
    // if one of the points is inside the rectangle
    if (
            (x1 >= this->x1 && x1 <= this->x2) && (y1 >= this->y1 && y1 <= this->y2)
            ) return true;

    if (
            (x2 >= this->x1 && x2 <= this->x2) && (y2 >= this->y1 && y2 <= this->y2)
            ) return true;

    float slope = (y2 - y1) / (x2 - x1);

    // top line

    float y = this->y1;
    float x = (y + slope * x1 - y1) / slope;
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

inline void Rect::setPosition(const Vecf &point) {
    float w = this->getWidth();
    float h = this->getHeight();
    x1 = point.x;
    y1 = point.y;
    x2 = point.x + w;
    y2 = point.y + h;
}

inline float Rect::getWidth() const {
    return x2 - x1;
}

inline float Rect::getHeight() const {
    return y2 - y1;
}

inline void Rect::copy(Rect rect) {
    this->x1 = rect.x1;
    this->y1 = rect.y1;
    this->x2 = rect.x2;
    this->y2 = rect.y2;
}

inline bool Rect::hasPointInside(const Vecf &point) const {
    return (point.x >= this->x1 && point.x <= this->x2) &&
           (point.y >= this->y1 && point.y <= this->y2);
}

#endif //__Rect_H_
