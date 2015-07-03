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
#include "WorldMap.h"
#include "Property.h"

PhysicsWorld::PhysicsWorld(
        Vector gForce,
        double metersPerPixel,
        double airDensity
):
    gForce(             Property<Vector>    (&gForce_)           ),
    metersPerPixel(     Property<double>    (&metersPerPixel_)   ),
    airDensity(         Property<double>    (&airDensity_)       ),

    gForce_(gForce)
{
    metersPerPixel_ = metersPerPixel;
    airDensity_ = airDensity;

    this->map_ = nullptr;
}

PhysicsWorld::~PhysicsWorld() {

}

void PhysicsWorld::add(Body *body) {
    bodies_.push_back(body);
    body->setWorld_(this);
}

void PhysicsWorld::step(double timeSeconds) {
    if (timeSeconds == 0.0) return;

    std::list<Body*> deadBodies;

    // update velocities and new locations
    for(std::list<Body *>::iterator it = bodies_.begin(); it != bodies_.end(); it++) {
        if ((*it)->isDead_) {
            deadBodies.push_back((*it));
        }
        else {
            (*it)->step_(timeSeconds);
        }
    }

    for(std::list<Body *>::iterator it = deadBodies.begin(); it != deadBodies.end(); it++) {
        bodies_.remove((*it));
        (*it)->onDie();
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

void PhysicsWorld::setMap(WorldMap * map) {
    this->map_ = map;
}

WorldMap *PhysicsWorld::getMap() {
    return this->map_;
}
