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


#include "WalkingEnemyAI.h"
#include "EnemyAIBase.h"
#include "Prefabs.h"
#include "ColliderLayers.h"

void EnemyAIBase::awake() {
    body_ = gameObject()->getComponent<Body>();
}

Transform *EnemyAIBase::getTarget() const {
    return target_;
}

void EnemyAIBase::setTarget(Transform *target) {
    target_ = target;
}

void EnemyAIBase::shootTarget_(bool useTimer) {
    if (shootingTimer_.resetIfTimeIntervalPassed() || !useTimer) {
        Vecf direction = target_->getPosition() - transform()->getPosition();
        auto laser = Prefabs::laser();

        laser->transform().setPosition(transform()->getPosition() + direction.normalized());
        laser->transform().setRotation(direction.angle());

        auto laserBody = laser->getComponent<Body>();
        laserBody->setVelocity(direction.normalized() * 20000.0 + body_->getVelocity());

        auto laserCollider = laser->getComponent<PolygonCollider>();
        laserCollider->setLayer(ColliderLayers::enemyAmmo);
    }
}

void EnemyAIBase::setSpawnerBehaviour(SpawnerBehaviour *spawnerBehaviour) {
    assert(spawnerBehaviour != nullptr);
    spawnerBehaviour_ = spawnerBehaviour;
}

void EnemyAIBase::update() {
    float distanceSqr = (target_->getPosition() - transform()->getPosition()).lengthSqr();

    if (distanceSqr > (maxDistance_ * maxDistance_)) {
        gameObject()->destroy();
    }
}

void EnemyAIBase::onDestroy() {
    Tile2DBehaviour::onDestroy();
    if (spawnerBehaviour_ != nullptr) {
        spawnerBehaviour_->remove(gameObject());
    }
}

const Timer &EnemyAIBase::getShootingTimer() const {
    return shootingTimer_;
}

void EnemyAIBase::setShootingTimer(const Timer &shootingTimer) {
    shootingTimer_ = shootingTimer;
}

float EnemyAIBase::getMaxDistance() const {
    return maxDistance_;
}

void EnemyAIBase::setMaxDistance(float maxDistance) {
    maxDistance_ = maxDistance;
}
