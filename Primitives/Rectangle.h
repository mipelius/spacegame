//
// Created by Miika Pelkonen on 1/15/14.
// Copyright (c) 2014 ___MIIKAPELKONEN___. All rights reserved.
//


#include "Point.h"

#ifndef __Rectangle_H_
#define __Rectangle_H_


class Rectangle {
private:
    Point firstPoint;
    Point secondPoint;
public:
    Rectangle(Point firstPoint, Point secondPoint);
    Point getTopLeftCorner();
    Point getTopRightCorner();
    Point getBottomLeftCorner();
    Point getBottomRightCorner();
    Point getFirstPoint();
    Point getSecondPoint();
    double getWidth();
    double getHeight();
    bool intersectsWith(Rectangle otherRectangle);
    bool intersectsWithLine(double x1, double y1, double x2, double y2);
};


#endif //__Rectangle_H_
