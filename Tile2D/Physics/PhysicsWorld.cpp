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

#include "gl_utils.h"
#include "Body.h"

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
    // remove removed bodies from the list
    for (auto* body : bodiesToRemove_) {
        bodies_.remove(body);
    }
    bodiesToRemove_.clear();

    if (timeSeconds == 0.0) return;

    // update velocities and new locations
    for(auto& body : bodies_) {
        body->step_(timeSeconds);
    }

    // now all the new positions are updated -> detect collision
    for(auto& body : bodies_) {
        detectCollision_(body, timeSeconds);
    }
}

void PhysicsWorld::detectCollision_(Body* body, double deltaTime) {
    body->detectMapCollision_(deltaTime);

    for (auto& bodyCur : bodies_) {
        if (bodyCur == body) continue;
        body->detectCollisionWith_(*bodyCur);
    }
}

void PhysicsWorld::remove(Body *body) {
    bodiesToRemove_.push_back(body);
    body->setWorld_(nullptr);
}

void PhysicsWorld::debugDraw() {
    prepareRendering();

    for (auto& body : bodies_) {
        if (body->collider_ == nullptr) {
            continue;
        }

        // COLLIDER

        glColor3f(0.3f, 1.0f, 0.3f);

        auto& points = body->collider_->points();

        glBegin(GL_LINE_LOOP);
        for (auto& pointOrig : points) {
            auto point = pointOrig.rotated(body->angle.get()) + body->position.get();
            glVertex2d(point.x, point.y);
        }
        glEnd();

        // BOUNDING BOX

        glColor3f(0.0f, 0.3f, 0.0f);

        Rect rect = body->collider_->boundingBox();
        rect.x1 += body->position.get().x;
        rect.y1 += body->position.get().y;
        rect.x2 += body->position.get().x;
        rect.y2 += body->position.get().y;

        glBegin(GL_LINE_LOOP);
            glVertex2d(rect.x1, rect.y1);
            glVertex2d(rect.x2, rect.y1);
            glVertex2d(rect.x2, rect.y2);
            glVertex2d(rect.x1, rect.y2);
        glEnd();
    }
}