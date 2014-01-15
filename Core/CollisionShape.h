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

#ifndef __CollisionShape_H_
#define __CollisionShape_H_

#include "Point.h"
#include "Rectangle.h"

class CollisionShape {
private:
    Rectangle boundingBox;
    Point *points;
    Point location;
    double angle;
    int count;
    bool intersectsWithHalfLine(Point linePoint1, Point linePoint2, Point offset);
public:
    CollisionShape(Point points[], int count);
    bool intersectsWith(CollisionShape *shape);
    void setLocation(Point point);
    Point getLocation();
    Point* getPoints();
    int getCount();
    Point *getRotatedPoints();
    Rectangle getBoundingBox();
    void setAngle(double angle);
};


#endif //__CollisionShape_H_
