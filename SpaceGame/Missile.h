//
// Created by Miika Pelkonen on 1/15/14.
// Copyright (c) 2014 ___MIIKAPELKONEN___. All rights reserved.
//


#include "Point.h"
#include "GameObjectGroup.h"

#ifndef __Missile_H_
#define __Missile_H_


class Missile : public GameObjectGroup {
private:
    static void onMissileCollision(GameEntity *gameEntity, CollisionEventArgs *args);
    double timeAlive;
    double missileAngle;
public:
    Missile(Point location, double angle, double forceAmount, Vector initialSpeed);
    bool detectCollisionWith(GameEntity* entity);
    void beforeStep(double timeElapsedSec);
};


#endif //__Missile_H_
