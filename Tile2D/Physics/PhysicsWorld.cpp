// MIT License
//
// This file is part of SpaceGame.
// Copyright (c) 2014-2018 Miika Pelkonen
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


#include "gl_utils.h"
#include "PhysicsWorld.h"
#include "GameObject.h"

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
                    !colliderLayerMatrix_->getRule(*collider->layer_, *otherCollider->layer_)
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

        Color color = layerColors[collider->layer_->id % layerColorsCount];
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

void PhysicsWorld::init(const std::string& colliderLayersFile) {
    colliderLayerMatrix_ = new ColliderLayerMatrix(colliderLayersFile);
}

PhysicsWorld::~PhysicsWorld() {
    delete colliderLayerMatrix_;
}

const ColliderLayerMatrix& PhysicsWorld::getColliderLayerMatrix() const {
    return *colliderLayerMatrix_;
}
