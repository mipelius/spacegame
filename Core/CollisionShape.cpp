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

#include <list>
#include <iostream>
#include <math.h>
#include "CollisionShape.h"

CollisionShape::CollisionShape(Point points[], int count): location(Point(0, 0)) {
    this->points = (Point*)malloc(count * sizeof(Point));
    for (int i=0; i<count; i++) {
        this->points[i] = points[i];
    }
    this->count = count;
}

bool CollisionShape::intersectsWith(CollisionShape* otherShape) {
    Point *otherShapePoints = otherShape->getRotatedPoints();
    Point *thisPoints = this->getRotatedPoints();

    // if one of the corners (points) of the other collision shape is inner side of this shape, collision has happened
    for (int i=0; i<otherShape->getCount(); i++) {
        // go trough all the points in this shape
        int intersectionCount = 0;
        for (int j=0; j<count-1; j++) {
            if (
                    intersectsWithHalfLine(
                            thisPoints[j] + Vector(location.x, location.y),
                            thisPoints[j+1] + Vector(location.x, location.y),
                            otherShapePoints[i] + Vector(otherShape->getLocation().x, otherShape->getLocation().y))
                    ) {

                intersectionCount++;
            }
        }
        // check also the line between the last and the first point
        if (
                intersectsWithHalfLine(
                        thisPoints[count-1] + Vector(location.x, location.y),
                        thisPoints[0] + Vector(location.x, location.y),
                        otherShapePoints[i] + Vector(otherShape->getLocation().x, otherShape->getLocation().y))
                ) {

            intersectionCount++;
        }

        // if the count is odd the collision has happened
        if (intersectionCount % 2) return true;
    }

    delete thisPoints;
    delete otherShapePoints;

    return false;
}

Point* CollisionShape::getPoints() {
    return this->points;
}

int CollisionShape::getCount() {
    return count;
}

bool CollisionShape::intersectsWithHalfLine(Point linePoint1, Point linePoint2, Point offset) {
    if (!(
            (linePoint1.y <= offset.y && linePoint2.y >= offset.y) ||
                    (linePoint1.y >= offset.y && linePoint2.y <= offset.y)
    )) return false;

    double collisionX;

    if (linePoint1.x == linePoint2.x) collisionX = linePoint1.x;
    else {
        double slope = (linePoint2.y - linePoint1.y) / (linePoint2.x - linePoint1.x);
        collisionX = (offset.y + slope * linePoint1.x - linePoint1.y) / slope;
    }
    return collisionX >= offset.x;
}

void CollisionShape::setLocation(Point point) {
    this->location = point;
}

Point CollisionShape::getLocation() {
    return location;
}

Point* CollisionShape::getRotatedPoints() {
    Point* points = (Point*)malloc(count * sizeof(Point));
    double angleRad = -this->angle / 360 * 2 * M_PI;
    for (int i=0; i<count; i++) {
        points[i].x = this->points[i].x * cos(angleRad) + this->points[i].y * sin(angleRad);
        points[i].y = this->points[i].x * -sin(angleRad) + this->points[i].y * cos(angleRad);
    }
    return points;
}

void CollisionShape::setAngle(double angle) {
    this->angle = angle;
}
