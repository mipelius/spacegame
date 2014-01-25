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

#include "Point.h"

#ifndef __Rect_H_
#define __Rect_H_

class Rect {
private:
    Point firstPoint;
    Point secondPoint;
public:
    Rect(Point firstPoint, Point secondPoint);
    Point getTopLeftCorner();
    Point getTopRightCorner();
    Point getBottomLeftCorner();
    Point getBottomRightCorner();
    Point getFirstPoint();
    Point getSecondPoint();
    double getWidth();
    double getHeight();
    bool intersectsWith(Rect otherRectangle);
    bool intersectsWithLine(double x1, double y1, double x2, double y2);
};


#endif //__Rect_H_
