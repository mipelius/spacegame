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
    lastJumpTimeStamp_ = SDL_GetTicks();
    lastReactionTimeStamp_ = SDL_GetTicks();

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

        Uint32 ticks = SDL_GetTicks();

        Uint32 reactionTime = reactionTime_ + (reactionTimeRandomness_ - rand() % (2 * reactionTimeRandomness_));
        if (ticks - lastReactionTimeStamp_ > reactionTime) {
            lastReactionTimeStamp_ = ticks;
            xDirection_ = (target_->getPosition().x - transform()->getPosition().x);
            xDirection_ = xDirection_ / abs(xDirection_);
        }

        vel.x = xDirection_ * 1000;

        Uint32 jumpInterval = jumpInterval_ + (jumpIntervalRandomness_ - rand() % (2 * jumpIntervalRandomness_));
        if (ticks - lastJumpTimeStamp_ > jumpInterval) {
            lastJumpTimeStamp_ = ticks;
            vel.y = -1000;
            transform()->setPosition(transform()->getPosition() + Vecf{0.0f, -1.0f});
        }

        body_->setVelocity(vel);
    }
}

void WalkingEnemyAI::shootTarget_() {
    Vecf direction = target_->getPosition() - transform()->getPosition();
}
