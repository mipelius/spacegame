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

#include "Rect.h"
#include "Vector.h"

class CollisionShape {
friend class Body;
private:
    Rect boundingBox;
    Vector *points;
    Body *owner;
    int count;
    bool intersectsWithHalfLine(Vector linePoint1, Vector linePoint2, Vector offset) const;
public:
    /**
     * Constructor that initializes the collision shape. Note: use only convex shapes.
     *
     * @param points[]  the points that defines the shape
     * @param count     the count of the points
     */
    CollisionShape(Vector points[], int count);

    /**
     * Checks if the shape intersects with the other shape
     *
     * @param shape     the other shape
     * @return bool     true if this shape intersects with the other shape, false otherwise
     */
    bool intersectsWith(const CollisionShape& shape) const;

    /**
     * Checks if the shape intersects with the given rectangle
     *
     * @param rectangle the rectangle
     * @return bool     true if this shape intersects with the given rectangle, false otherwise
     */

    bool intersectsWith(const Rect& rectangle) const;

    /**
     * Gets the points of the CollisionShape
     *
     * @return the points
     */
    Vector* getPoints();

    /**
     * Gets the count of the points of the CollisionShape
     *
     * @return the count of the points
     */
    int getCount() const;

    /**
     * Gets the rotated points of the CollisionShape
     *
     * @return the points
     */
    const Vector * getRotatedPoints() const;

    /**
     * Gets the bounding box which. Note: the location is already summed to points in this rectangle
     *
     * @return the bounding box
     */
    Rect getBoundingBox() const;
};


#endif //__CollisionShape_H_
