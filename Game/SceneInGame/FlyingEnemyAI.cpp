// This file is part of SpaceGame.
// Copyright (C) 2018 Miika Pelkonen
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

    float distanceToTarget = (target_->getPosition() - transform()->getPosition()).length();

    bool canSeeTarget = canSeeTarget_();

    if (
            (distanceToTarget < minPathFindingDistance_ && canSeeTarget) ||
            distanceToTarget > maxPathFindingDistance_
    ) {
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
        float currentRotation = transform()->getRotation();
        float targetRotation = movement.angle();
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

        if (fabsf(deltaAngle) < 45.0f) {
            transform()->setPosition(currentPosition + movement);
        }
    } else {
        transform()->setPosition(currentPosition + movement);
    }
}

void FlyingEnemyAI::lateUpdate() {

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

const Timer &FlyingEnemyAI::getPathUpdateTimer() const {
    return pathUpdateTimer_;
}

void FlyingEnemyAI::setPathUpdateTimer(const Timer &pathUpdateTimer_) {
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

