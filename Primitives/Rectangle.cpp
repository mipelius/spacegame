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


