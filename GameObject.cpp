//
// Created by Miika Pelkonen on 1/4/14.
// Copyright (c) 2014 ___MIIKAPELKONEN___. All rights reserved.
//

#include "GameObject.h"

GameObject::GameObject(
        Point focus,
        Point location,
        Texture *texture,
        CollisionShape *collisionShape,
        double mass
) : GameEntity(focus, location) {
    this->texture = texture;
    this->collisionShape = collisionShape;
    this->mass = mass;
}