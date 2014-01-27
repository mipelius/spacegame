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

#include "precompile.h"
#include "CollisionShape.h"
#include "GameEntity.h"
#include "Rect.h"

CollisionShape::CollisionShape(Point points[], int count): boundingBox(Rect(Point(-1, -1), Point(1, 1)))
{
    this->points = (Point*)malloc(count * sizeof(Point));
    this->count = count;

    Point farMost = Point(0, 0);

    for (int i=0; i<count; i++) {
        this->points[i] = points[i];

        double length = Vector(points[i].x, points[i].y).length();
        double farMostLength = Vector(farMost.x, farMost.y).length();
        if (length > farMostLength) farMost = points[i];
    }

    double length = Vector(farMost.x, farMost.y).length();
    boundingBox = Rect(Point(-length, -length), Point(length, length));
}

bool CollisionShape::intersectsWith(CollisionShape* otherShape) {
    if (!this->getBoundingBox().intersectsWith(otherShape->getBoundingBox())) return false;

    Point location = owner->getLocation();

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
                            otherShapePoints[i] + Vector(otherShape->owner->getLocation().x, otherShape->owner->getLocation().y))
                    ) {

                intersectionCount++;
            }
        }
        // check also the line between the last and the first point
        if (
                intersectsWithHalfLine(
                        thisPoints[count-1] + Vector(location.x, location.y),
                        thisPoints[0] + Vector(location.x, location.y),
                        otherShapePoints[i] + Vector(otherShape->owner->getLocation().x, otherShape->owner->getLocation().y))
                ) {

            intersectionCount++;
        }

        // if the count is odd the collision has happened
        if (intersectionCount % 2) return true;
    }

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

Point* CollisionShape::getRotatedPoints() {
    double angleRad = this->owner->getAngle() / 360 * 2 * M_PI * -1;

    Point* rotatedPoints = (Point*)malloc(count * sizeof(Point));

    for (int i=0; i<count; i++) {
        rotatedPoints[i].x = this->points[i].x * cos(angleRad) + this->points[i].y * sin(angleRad);
        rotatedPoints[i].y = -this->points[i].x * sin(angleRad) + this->points[i].y * cos(angleRad);
    }
    return rotatedPoints;
}

Rect CollisionShape::getBoundingBox() {
    Point location = owner->getLocation();

    return Rect(
            Point(
                    this->boundingBox.getTopLeftCorner().x + location.x,
                    this->boundingBox.getTopLeftCorner().y + location.y
            ),
            Point(
                    this->boundingBox.getBottomRightCorner().x + location.x,
                    this->boundingBox.getBottomRightCorner().y + location.y
            )
    );
}


bool CollisionShape::intersectsWith(Rect* rectangle) {
    Point location = owner->getLocation();

    Point* points = getRotatedPoints();

    for (int i=0; i<count-1; i++) {
        if (rectangle->intersectsWithLine(
                points[i].x + location.x,
                points[i].y + location.y,
                points[i+1].x + location.x,
                points[i+1].y + location.y)
                ) return true;
    }

    if (rectangle->intersectsWithLine(
            points[count-1].x + location.x,
            points[count-1].y + location.y,
            points[0].x + location.x,
            points[0].y + location.y)
            ) return true;

    return false;
}
