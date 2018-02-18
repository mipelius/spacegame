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
#include "PhysicsWorld.h"

PhysicsWorld::PhysicsWorld():
    gForce_(defaultGForce)
{
    airDensity_ = defaultAirDensity;
}

void PhysicsWorld::step(float timeSeconds) {
    if (timeSeconds == 0.0) {
        return;
    }

    // update velocities and positions
    for(auto& body : bodies_) {
        if (body->gameObject()->isActive()) {
            body->step_(timeSeconds);
        }
    }

    // now all the positions are updated -> detect collision
    for(auto& collider : colliders_) {
        if (!collider->gameObject()->isActive()) {
            continue;
        }
        collider->detectTerrainCollision_(timeSeconds);

        for (auto& otherCollider : colliders_) {
            if (
                    !otherCollider->gameObject()->isActive() ||
                    otherCollider == collider ||
                    !colliderLayerMatrix_->getRule(collider->layer_, otherCollider->layer_)
            ) {
                continue;
            }

            collider->detectCollisionWith_(*otherCollider);
        }
    }
}

void PhysicsWorld::debugDraw() {
    static const int layerColorsCount = 9;
    static const Color layerColors[layerColorsCount] = {
            {1.0, 0.0, 0.0},
            {0.0, 1.0, 0.0},
            {0.0, 0.0, 1.0},
            {1.0, 1.0, 0.0},
            {0.0, 1.0, 1.0},
            {1.0, 0.0, 1.0},
            {1.0, 0.5, 0.5},
            {0.5, 1.0, 0.5},
            {0.5, 0.5, 1.0},
    };

    prepareRendering();

    for (auto& collider : colliders_) {
        if (collider == nullptr) {
            continue;
        }

        // COLLIDER

        Color color = layerColors[collider->layer_ % layerColorsCount];
        glColor3f(color.red, color.green, color.blue);

        auto& points = collider->points();

        glBegin(GL_LINE_LOOP);
        for (auto& pointOrig : points) {
            auto point = pointOrig.rotated(collider->transform()->getRotation()) + collider->transform()->getPosition();
            glVertex2f(point.x, point.y);
        }
        glEnd();

        // BOUNDING BOX

        glColor3f(0.0f, 0.3f, 0.0f);

        Rect rect = collider->boundingBox();
        rect.x1 += collider->transform()->getPosition().x;
        rect.y1 += collider->transform()->getPosition().y;
        rect.x2 += collider->transform()->getPosition().x;
        rect.y2 += collider->transform()->getPosition().y;

        glBegin(GL_LINE_LOOP);
            glVertex2f(rect.x1, rect.y1);
            glVertex2f(rect.x2, rect.y1);
            glVertex2f(rect.x2, rect.y2);
            glVertex2f(rect.x1, rect.y2);
        glEnd();
    }
}

// getters and setters

const Vecf &PhysicsWorld::getGForce() const {
    return gForce_;
}

void PhysicsWorld::setGForce(const Vecf &gForce) {
    gForce_ = gForce;
}

float PhysicsWorld::getAirDensity() const {
    return airDensity_;
}

void PhysicsWorld::setAirDensity(float airDensity) {
    airDensity_ = airDensity;
}

void PhysicsWorld::init(std::vector<ColliderLayerMatrix::Rule> colliderMatrixRules) {
    colliderLayerMatrix_ = new ColliderLayerMatrix(colliderMatrixRules);
}

PhysicsWorld::~PhysicsWorld() {
    delete colliderLayerMatrix_;
}
