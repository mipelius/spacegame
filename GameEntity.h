//
// Created by Miika Pelkonen on 1/4/14.
// Copyright (c) 2014 ___MIIKAPELKONEN___. All rights reserved.
//


#include "Point.h"
#include "Vector.h"

#ifndef __GameEntity_H_
#define __GameEntity_H_


class GameEntity {
private:
    Point location;
    double angle;
    Point focus;
    Vector force;
public:
    GameEntity(Point focus, Point location);
    void applyForce(Vector force);
    void setAngle(double angle);
    double getAngle();
    void turnClockwise();
    void turnCounterClockwise();
    void setLocation(Point location);
    Point getLocation();
    void setFocus(Point location);
    Point getFocus();
};


#endif //__GameEntity_H_
