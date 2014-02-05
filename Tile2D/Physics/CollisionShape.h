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

class GameEntity;

#include "Rect.h"
#include "Point.h"

class CollisionShape {
friend class GameEntity;
private:
    Rect boundingBox;
    Point *points;
    GameEntity *owner;
    int count;
    bool intersectsWithHalfLine(Point linePoint1, Point linePoint2, Point offset);
public:
    /**
     * Constructor that initializes the collision shape. Note: use only convex shapes.
     *
     * @param points[]  the points that defines the shape
     * @param count     the count of the points
     */
    CollisionShape(Point points[], int count);

    /**
     * Checks if the shape intersects with the other shape
     *
     * @param shape     the other shape
     * @return bool     true if this shape intersects with the other shape, false otherwise
     */
    bool intersectsWith(CollisionShape *shape);

    /**
     * Checks if the shape intersects with the given rectangle
     *
     * @param rectangle the rectangle
     * @return bool     true if this shape intersects with the given rectangle, false otherwise
     */

    bool intersectsWith(Rect* rectangle);

    /**
     * Gets the points of the CollisionShape
     *
     * @return the points
     */
    Point* getPoints();

    /**
     * Gets the count of the points of the CollisionShape
     *
     * @return the count of the points
     */
    int getCount();

    /**
     * Gets the rotated points of the CollisionShape
     *
     * @return the points
     */
    Point *getRotatedPoints();

    /**
     * Gets the bounding box which. Note: the location is already summed to points in this rectangle
     *
     * @return the bounding box
     */
    Rect getBoundingBox();
};


#endif //__CollisionShape_H_
