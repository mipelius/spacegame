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
#include "TileMap.h"

PhysicsWorld::PhysicsWorld():
    gForce(             Property<Vec>    (&gForce_)           ),
    metersPerPixel(     Property<double>    (&metersPerPixel_)   ),
    airDensity(         Property<double>    (&airDensity_)       ),

    map_(nullptr),
    gForce_({0, 0})
{
    gForce_ = defaultGForce;
    metersPerPixel_ = defaultMetersPerPixel;
    airDensity_ = defaultAirDensity;
}

void PhysicsWorld::add(Body *body) {
    bodies_.push_back(body);
    body->setWorld_(this);
}

void PhysicsWorld::step(double timeSeconds) {
    if (timeSeconds == 0.0) return;

    // update velocities and new locations
    for(auto& body : bodies_) {
        body->step_(timeSeconds);
    }

    // now all the new positions are updated -> detect collision
    for(auto& body : bodies_) {
        detectCollision_(*body);
    }
}

void PhysicsWorld::detectCollision_(Body &body) {
    body.detectMapCollision_();

    for (auto& bodyCur : bodies_) {
        if (bodyCur == &body) continue;
        body.detectCollisionWith_(*bodyCur);
    }
}

void PhysicsWorld::remove(Body *body) {
    bodies_.remove(body);
    body->setWorld_(nullptr);
}
