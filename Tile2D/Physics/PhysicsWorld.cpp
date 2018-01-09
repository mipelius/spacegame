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

#include "Tile2D.h"
#include "Canvas.h"
#include "Camera.h"
#include "precompile.h"
#include "PhysicsWorld.h"
#include "Body.h"
#include "ColliderShape.h"
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
    bodies_.remove(body);
    body->setWorld_(nullptr);
}

void PhysicsWorld::debugDraw() {
    Canvas& canvas = Tile2D::canvas();
    auto& camera = Tile2D::canvas().getCamera();
    Rect rect = canvas.getRenderingAreaRect();

    auto x = (GLint)(rect.x1);
    auto y = (GLint)(Tile2D::window().h.get() - rect.y2);
    auto w = (GLint)(rect.getWidth());
    auto h = (GLint)(rect.getHeight());

    glViewport(x, y, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(
            camera.areaRect.get().x1,
            camera.areaRect.get().x2,
            camera.areaRect.get().y2,
            camera.areaRect.get().y1,
            -1.0,
            1.0
    );
    glMatrixMode(GL_MODELVIEW);

    glColor3f(0.3f, 1.0f, 0.3f);

    for (auto& body : bodies_) {
        if (body->collider == nullptr) {
            continue;
        }
        auto points = body->collider->points;
        glBegin(GL_LINE_STRIP);
        for (auto& pointOrig : points) {
            auto point = pointOrig.rotated(body->angle.get());
            glVertex2f(point.x + body->position_.x, point.y + body->position_.y);
        }
        auto point = points[0].rotated(body->angle.get());
        glVertex2f(point.x + body->position_.x, point.y + body->position_.y);

        Rect rect = body->collider->boundingBox();

        glEnd();

        glBegin(GL_LINE_STRIP);
        glVertex2f(rect.x1 + body->position.get().x, rect.y1 + body->position.get().y);
        glVertex2f(rect.x2 + body->position.get().x, rect.y1 + body->position.get().y);
        glVertex2f(rect.x2 + body->position.get().x, rect.y2 + body->position.get().y);
        glVertex2f(rect.x1 + body->position.get().x, rect.y2 + body->position.get().y);

        glEnd();


    }
}