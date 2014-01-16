//
// Created by Miika Pelkonen on 1/15/14.
// Copyright (c) 2014 ___MIIKAPELKONEN___. All rights reserved.
//

#include "Rectangle.h"

Rectangle::Rectangle(Point firstPoint, Point secondPoint): firstPoint(firstPoint), secondPoint(secondPoint) {
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

Point Rectangle::getTopLeftCorner() {
    return firstPoint;
}

Point Rectangle::getTopRightCorner() {
    return Point(secondPoint.x, firstPoint.y);
}

Point Rectangle::getBottomLeftCorner() {
    return Point(firstPoint.x, secondPoint.y);
}

Point Rectangle::getBottomRightCorner() {
    return secondPoint;
}

double Rectangle::getWidth() {
    return secondPoint.x - firstPoint.x;
}

double Rectangle::getHeight() {
    return secondPoint.y - firstPoint.y;
}

bool Rectangle::intersectsWith(Rectangle otherRectangle) {
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

Point Rectangle::getFirstPoint() {
    return firstPoint;
}

Point Rectangle::getSecondPoint() {
    return secondPoint;
}

bool Rectangle::intersectsWithLine(double x1, double y1, double x2, double y2) {
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
