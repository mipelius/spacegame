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
#include "Tile2DMath.h"
#include "Tile2D.h"

void WalkingEnemyAI::awake() {
    jumpTimer_.setInterval(6000);
    jumpTimer_.setIntervalRandomness(1000);

    reactionTimer_.setInterval(500);
    reactionTimer_.setIntervalRandomness(200);

    shootTimer_.setInterval(1000);
    shootTimer_.setIntervalRandomness(200);

    body_ = gameObject()->getComponent<Body>();
}

void WalkingEnemyAI::update() {
    if (target_ != nullptr) {
        walkTowardsTarget_();
        shootTarget_();
    }
}

void WalkingEnemyAI::lateUpdate() { }

bool WalkingEnemyAI::isGrounded_() {
    for (auto groundCheckPoint : groundSensors_) {
        auto tile = Tile2D::tileMap().getValueScaled(transform()->getPosition() + groundCheckPoint);
        if (tile != nullptr && tile->getDensity() > Mathf::epsilon) {
            return true;
        }
    }
    return false;
}

void WalkingEnemyAI::setGroundCheckSensors(const std::vector<Vecf> &groundSensors) {
    groundSensors_ = groundSensors;
}

void WalkingEnemyAI::walkTowardsTarget_() {
    if (isGrounded_()) {
        Vecf vel = body_->getVelocity();

        if (reactionTimer_.resetIfTimeIntervalPassed()) {
            xDirection_ = (target_->getPosition().x - transform()->getPosition().x);
            xDirection_ = xDirection_ / abs(xDirection_);
        }

        vel.x = xDirection_ * 1000;

        if (jumpTimer_.resetIfTimeIntervalPassed()) {
            vel.y = -1000;
            transform()->setPosition(transform()->getPosition() + Vecf{0.0f, -1.0f});
        }

        body_->setVelocity(vel);
    }
}

void WalkingEnemyAI::shootTarget_() {
    if (shootTimer_.resetIfTimeIntervalPassed()) {
        Vecf direction = target_->getPosition() - transform()->getPosition();
        // shoot to direction
    }
}