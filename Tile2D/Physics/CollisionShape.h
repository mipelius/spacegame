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

class Body;

#include "Tile2DObject.h"
#include "Rect.h"
#include "Vec.h"
#include <vector>
#include "Property.h"
#include "Body.h"

class CollisionShape : public Tile2DObject {
friend class Body;
private:
    Rect boundingBox_;
    std::vector<Vec> points_;
    Body *owner_;
    bool intersectsWithHalfLine(const Vec &linePoint1, const Vec &linePoint2, const Vec &offset) const;

public:
    const Property<std::vector<Vec> > points;

    /**
     * Constructor that initializes the collision shape. Note: use only convex shapes.
     *
     * @param points[]  the points that defines the shape
     * @param count     the count of the points
     */
    CollisionShape();

    /**
     * Checks if the shape intersects with the other shape
     *
     * @param shape     the other shape
     * @return bool     true if this shape intersects with the other shape, false otherwise
     */
    bool intersectsWith(const CollisionShape* shape) const;

    /**
     * Checks if the shape intersects with the given rectangle
     *
     * @param rectangle the rectangle
     * @return bool     true if this shape intersects with the given rectangle, false otherwise
     */

    bool intersectsWith(const Rect& rectangle) const;

    /**
     * Gets the rotated points of the CollisionShape
     *
     * @return the points
     */
    inline std::vector<Vec> getRotatedPoints() const;

    /**
     * Gets the bounding box which. Note: the location is already summed to points in this rectangle
     *
     * @return the bounding box
     */
    Rect getBoundingBox() const;

    static std::vector<Vec> getPoints_(void *owner);
    static void setPoints_(void *owner, const std::vector<Vec>& value);
};

// inline functions

inline std::vector<Vec> CollisionShape::getRotatedPoints() const {
    double angleRad = owner_->angle.get() / 360 * 2 * M_PI * -1;

    std::vector<Vec> rotatedPoints(points_.size());

    for (int i=0; i<points_.size(); i++) {
        rotatedPoints[i].x = this->points_[i].x * cos(angleRad) + this->points_[i].y * sin(angleRad);
        rotatedPoints[i].y = -this->points_[i].x * sin(angleRad) + this->points_[i].y * cos(angleRad);
    }
    return rotatedPoints;
}


#endif //__CollisionShape_H_
