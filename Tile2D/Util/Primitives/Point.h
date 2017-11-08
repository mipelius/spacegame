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

#ifndef __Location_H_
#define __Location_H_

class Rect;

#include "Vector.h"

class Point {

public:
    double x, y;
    Point(double x, double y);
    ~Point();

    bool equals(Point otherPoint);

    Point operator + (const Vector& vector) const;
    Point operator - (const Vector& vector) const;
    double distance(const Point &otherPoint) const;

    bool isIn(const Rect& rect);
};


#endif //__Location_H_
