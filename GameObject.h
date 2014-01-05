//
// Created by Miika Pelkonen on 1/4/14.
// Copyright (c) 2014 ___MIIKAPELKONEN___. All rights reserved.
//


#include "GameEntity.h"
#include "CollisionShape.h"
#include "Texture.h"

#ifndef __GameObject_H_
#define __GameObject_H_


class GameObject : public GameEntity {
private:
    Texture* texture;
    CollisionShape* collisionShape;
    double mass;

public:
    GameObject(
            Point focus,
            Point location,
            Texture *texture,
            CollisionShape *collisionShape,
            double mass
    );

};


#endif //__GameObject_H_
