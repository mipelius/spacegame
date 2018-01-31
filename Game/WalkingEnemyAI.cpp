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

Transform *WalkingEnemyAI::getTarget() const {
    return target_;
}

void WalkingEnemyAI::setTarget(Transform *target) {
    target_ = target;
}

void WalkingEnemyAI::awake() {
    body_ = gameObject()->getComponent<Body>();
}

void WalkingEnemyAI::update() {
    if (target_ != nullptr) {
        Vecf vel = body_->getVelocity();

        float xChange = (target_->getPosition().x - transform()->getPosition().x);
        xChange = xChange / abs(xChange);

        float maxVelX = xChange * 1000.0f;

        if ((vel.x < 0 && maxVelX) < 0 || (vel.x > 0 && maxVelX > 0)) {
            if (abs(vel.x) > abs(maxVelX)) {
                return;
            }
        }

        xChange *= 100.0f;

        vel.x += xChange;

        body_->setVelocity(vel);
    }
}

void WalkingEnemyAI::lateUpdate() {

}
