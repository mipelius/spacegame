//
// Created by Miika Pelkonen on 1/5/14.
// Copyright (c) 2014 ___MIIKAPELKONEN___. All rights reserved.
//


#include "Point.h"

#ifndef __CollisionShape_H_
#define __CollisionShape_H_


class CollisionShape {
private:
    Point** points;
public:
    CollisionShape(Point** points);
    Point** getPoints();
};


#endif //__CollisionShape_H_
