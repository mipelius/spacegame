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
#include "PhysicsWorld.h"
#include "Body.h"
#include "Map.h"
#include "SimpleProperty.h"

PhysicsWorld::PhysicsWorld(
        Vector gForce,
        double metersPerPixel,
        double airDensity
):
    gForce(             new SimpleProperty<Vector>    (&gForce_)           ),
    metersPerPixel(     new SimpleProperty<double>    (&metersPerPixel_)   ),
    airDensity(         new SimpleProperty<double>    (&airDensity_)       ),

    gForce_(gForce)
{
    metersPerPixel_ = metersPerPixel;
    airDensity_ = airDensity;

    this->map_ = nullptr;
}

PhysicsWorld::~PhysicsWorld() {
    delete gForce;
    delete metersPerPixel;
    delete airDensity;
}

void PhysicsWorld::add(Body *gameEntity) {
    bodies_.push_back(gameEntity);
    gameEntity->setWorld_(this);
}

void PhysicsWorld::step(double timeSeconds) {
    if (timeSeconds == 0.0) return;

    // update velocities and new locations
    for(std::list<Body *>::iterator it = bodies_.begin(); it != bodies_.end(); it++) {
        (*it)->step_(timeSeconds);
    }

    // now all the new locations are updated -> detect collision
    for(std::list<Body *>::iterator it = bodies_.begin(); it != bodies_.end(); it++) {
        detectCollision_((*it));
    }
}

void PhysicsWorld::detectCollision_(Body *entity) {
    entity->detectMapCollision_();

    for (std::list<Body *>::iterator it = bodies_.begin(); it != bodies_.end(); it++) {
        if ((*it) == entity) continue;
        entity->detectCollisionWith_(*it);
    }
}

void PhysicsWorld::setMap(Map* map) {
    this->map_ = map;
}

Map *PhysicsWorld::getMap() {
    return this->map_;
}
