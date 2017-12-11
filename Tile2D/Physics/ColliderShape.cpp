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
#include "ColliderShape.h"

ColliderShape::ColliderShape():
        points( Property<std::vector<Vec> >(this, getPoints_, setPoints_   )),
        boundingBox_(Rect(-1, -1, 1, 1))
{
}

bool ColliderShape::intersectsWith(const ColliderShape* otherShape) const {
    if (!this->getBoundingBox().intersectsWith(otherShape->getBoundingBox())) return false;

    Vec location = owner_->position.get();

    std::vector<Vec> otherShapePoints = otherShape->getRotatedPoints();
    std::vector<Vec> thisPoints       = this->getRotatedPoints();

    // if one of the corners (points) of the other collision shape is inner side of this shape, collision has happened
    for (int i=0; i<otherShape->points.get().size(); i++) {
        // go trough all the points in this shape
        int intersectionCount = 0;
        for (int j=0; j<points_.size()-1; j++) {
            if (
                    intersectsWithHalfLine(
                            thisPoints[j] + Vec(location.x, location.y),
                            thisPoints[j+1] + Vec(location.x, location.y),
                            otherShapePoints[i] + Vec(otherShape->owner_->position.get().x, otherShape->owner_->position.get().y))
                    ) {

                intersectionCount++;
            }
        }
        // check also the line between the last and the first point
        if (
                intersectsWithHalfLine(
                        thisPoints[points_.size()-1] + Vec(location.x, location.y),
                        thisPoints[0] + Vec(location.x, location.y),
                        otherShapePoints[i] + Vec(otherShape->owner_->position.get().x, otherShape->owner_->position.get().y))
                ) {

            intersectionCount++;
        }

        // if the count is odd the collision has happened
        if (intersectionCount % 2 == 1) return true;
    }

    return false;
}

bool ColliderShape::intersectsWithHalfLine(const Vec &linePoint1, const Vec &linePoint2, const Vec &offset) const {
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

Rect ColliderShape::getBoundingBox() const {
    Vec location = owner_->position.get();

    return {
            this->boundingBox_.x1 + location.x,
            this->boundingBox_.y1 + location.y,
            this->boundingBox_.x2 + location.x,
            this->boundingBox_.y2 + location.y
    };
}

bool ColliderShape::intersectsWith(const Rect& rectangle) const {
    Vec location = owner_->position.get();

    std::vector<Vec> points = getRotatedPoints();

    for (int i=0; i<points_.size()-1; i++) {
        if (rectangle.intersectsWithLine(
                points[i].x + location.x,
                points[i].y + location.y,
                points[i+1].x + location.x,
                points[i+1].y + location.y)
                ) return true;
    }

    return
            rectangle.intersectsWithLine(
    points[points_.size()-1].x + location.x,
    points[points_.size()-1].y + location.y,
    points[0].x + location.x,
    points[0].y + location.y
            );

}

std::vector<Vec> ColliderShape::getPoints_(void *owner) {
    return ((ColliderShape*)owner)->points_;
}

void ColliderShape::setPoints_(void *owner, const std::vector<Vec>& value) {
    auto shape = (ColliderShape*)owner;
    shape->points_ = value;

    Vec farMost = Vec(0, 0);

    for (const auto& vec : value) {
        double length = vec.length();
        double farMostLength = Vec(farMost.x, farMost.y).length();
        if (length > farMostLength) farMost = vec;
    }

    double length = Vec(farMost.x, farMost.y).length();
    shape->boundingBox_ = Rect(-length, -length, length, length);
}
