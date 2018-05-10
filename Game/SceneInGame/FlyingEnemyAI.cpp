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


#include "Tile2DMath.h"
#include "FlyingEnemyAI.h"
#include "Tile2D.h"
#include "PathFinder.h"
#include "t2Time.h"

void FlyingEnemyAI::awake() {
    EnemyAIBase::awake();

    nextPoint_ = transform()->getPosition();

    collider_ = gameObject()->getComponent<PolygonCollider>();
}

void FlyingEnemyAI::update() {
    EnemyAIBase::update();
    shootTarget_();

    Vecf targetDirection = target_->getPosition() - transform()->getPosition();
    float distanceToTarget = targetDirection.length();

    bool canSeeTarget = canSeeTarget_();

    if (
            (distanceToTarget < minPathFindingDistance_ && canSeeTarget) ||
            distanceToTarget > maxPathFindingDistance_
    ) {
        if (rotates_) {
            rotateTowards_(targetDirection);
        }
        return;
    }

    if (canSeeTarget) {
        pathToTarget_.clear();
        pathToTarget_.push_back(target_->getPosition());
    }
    else if (pathUpdateTimer_.resetIfTimeIntervalPassed()) {
        pathToTarget_ = Tile2D::pathFinder().getPath(
                transform()->getPosition(),
                target_->getPosition(),
                maxNodesPathFinderExplores_,
                true,
                collider_->boundingBox()
        );
    }

    if (pathToTarget_.empty()) {
        return;
    }

    for (auto i = 0u; i < 10; ++i) {
        updateNextPoint_();
        if ((transform()->getPosition() - nextPoint_).length() > 8.0f) {
            break;
        }
    }

    Vecf currentPosition = transform()->getPosition();
    Vecf direction = (nextPoint_ - currentPosition).normalized();
    Vecf movement = direction * Tile2D::time().getDeltaTime() * speed;

    if (rotates_) {
        auto deltaAngle = rotateTowards_(movement);
        if (fabsf(deltaAngle) < 45.0f) {
            transform()->setPosition(currentPosition + movement);
        }
    } else {
        transform()->setPosition(currentPosition + movement);
    }
}

void FlyingEnemyAI::lateUpdate() {
    if (flips_) {
        Vecf scale = {
            transform()->getScale().x,
            abs(transform()->getScale().y)
        };

        if (abs(Mathf::deltaAngle(0.0f, transform()->getRotation())) > 90.0f) {
            scale.y *= -1.0f;
        }

        transform()->setScale(scale);
    }
}

void FlyingEnemyAI::updateNextPoint_() {
    if (pathToTarget_.empty()) {
        return;
    }
    auto it = pathToTarget_.begin();
    nextPoint_ = *it;
    pathToTarget_.erase(it);
}

float FlyingEnemyAI::getMaxPathFindingDistance() const {
    return maxPathFindingDistance_;
}

void FlyingEnemyAI::setMaxPathFindingDistance(float maxPathFindingDistance_) {
    FlyingEnemyAI::maxPathFindingDistance_ = maxPathFindingDistance_;
}

const CountDownTimer &FlyingEnemyAI::getPathUpdateTimer() const {
    return pathUpdateTimer_;
}

void FlyingEnemyAI::setPathUpdateTimer(const CountDownTimer &pathUpdateTimer_) {
    FlyingEnemyAI::pathUpdateTimer_ = pathUpdateTimer_;
}

unsigned int FlyingEnemyAI::getMaxNodesPathFinderExplores() const {
    return maxNodesPathFinderExplores_;
}

void FlyingEnemyAI::setMaxNodesPathFinderExplores(unsigned int maxNodesPathFinderExplores) {
    maxNodesPathFinderExplores_ = maxNodesPathFinderExplores;
}

float FlyingEnemyAI::getMinPathFindingDistance() const {
    return minPathFindingDistance_;
}

void FlyingEnemyAI::setMinPathFindingDistance(float minPathFindingDistance) {
    minPathFindingDistance_ = minPathFindingDistance;
}

float FlyingEnemyAI::getSpeed() const {
    return speed;
}

void FlyingEnemyAI::setSpeed(float speed) {
    FlyingEnemyAI::speed = speed;
}

float FlyingEnemyAI::getAngularSpeed() const {
    return angularSpeed_;
}

void FlyingEnemyAI::setAngularSpeed(float angularSpeed) {
    angularSpeed_ = angularSpeed;
}

bool FlyingEnemyAI::rotates() const {
    return rotates_;
}

void FlyingEnemyAI::setRotates(bool rotates) {
    rotates_ = rotates;
}

float FlyingEnemyAI::rotateTowards_(Vecf target) {
    float currentRotation = transform()->getRotation();
    float targetRotation = target.angle();
    float deltaAngle = Mathf::deltaAngle(currentRotation, targetRotation);

    if (!Mathf::approx(deltaAngle, 0.0f)) {
        auto angularDirection = (float)Mathf::sign(deltaAngle);
        float rotate = angularDirection * Tile2D::time().getDeltaTime() * angularSpeed_;
        if (fabsf(rotate) > fabsf(deltaAngle)) {
            transform()->setRotation(targetRotation);
        } else {
            transform()->setRotation(currentRotation + rotate);
        }
    }

    return deltaAngle;
}

bool FlyingEnemyAI::flips() const {
    return flips_;
}

void FlyingEnemyAI::setFlips(bool flips) {
    flips_ = flips;
}

