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


#include "WalkingEnemyAI.h"
#include "Tile2DMath.h"
#include "Tile2D.h"
#include "TileMap.h"
#include "GameObject.h"

void WalkingEnemyAI::awake() {
    EnemyAIBase::awake();
    jumpTimer_.setInterval(6000);
    jumpTimer_.setIntervalRandomness(1000);

    reactionTimer_.setInterval(500);
    reactionTimer_.setIntervalRandomness(200);

    body_ = gameObject()->getComponent<Body>();
}

void WalkingEnemyAI::update() {
    EnemyAIBase::update();
    if (target_ != nullptr) {
        if (reactionTimer_.resetIfTimeIntervalPassed()) {
            updateDirection_();
        }
        if (isGrounded_()) {
            walkTowardsTarget_();
        }

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

void WalkingEnemyAI::updateDirection_() {
    xDirection_ = (target_->getPosition().x - transform()->getPosition().x);
    xDirection_ = xDirection_ / abs(xDirection_);

    Vecf scale = transform()->getScale();

    if (xDirection_ > 0) {
        scale.x = abs(scale.x);
    } else {
        scale.x = -abs(scale.x);
    }

    transform()->setScale(scale);
}

void WalkingEnemyAI::walkTowardsTarget_() {
    Vecf vel = body_->getVelocity();

    vel.x = xDirection_ * 100;

    if (jumpTimer_.resetIfTimeIntervalPassed()) {
        vel.y = -100;
        transform()->setPosition(transform()->getPosition() + Vecf{0.0f, -1.0f});
    }

    body_->setVelocity(vel);
}
