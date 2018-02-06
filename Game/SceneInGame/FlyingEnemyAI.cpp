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


#include "FlyingEnemyAI.h"
#include "Tile2D.h"

void FlyingEnemyAI::awake() {
    EnemyAIBase::awake();

    pathUpdateTimer_.setInterval(1000);
    pathUpdateTimer_.setIntervalRandomness(0);
    pathUpdateTimer_.reset();

    shootingTimer_.setInterval(500);
    shootingTimer_.setIntervalRandomness(200);

    nextPoint_ = transform()->getPosition();
    lastPoint_ = nextPoint_;

    collider_ = gameObject()->getComponent<PolygonCollider>();
}

void FlyingEnemyAI::update() {
    float distanceToTarget = (target_->getPosition() - transform()->getPosition()).length();

    if (pathUpdateTimer_.timeIntervalPassed() && distanceToTarget < maxPathFindingDistance_) {
        pathUpdateTimer_.reset();

        pathToTarget_ = Tile2D::pathFinder().getPath(
                transform()->getPosition(),
                target_->getPosition(),
                collider_->boundingBox(),
                true
        );
        updateNextPoint_();
    }

    if ((transform()->getPosition() - nextPoint_).length() < 16.0f) {
        updateNextPoint_();
    }

    Vecf currentPosition = transform()->getPosition();
    Vecf movement = (nextPoint_ - lastPoint_) / 2;

    transform()->setPosition(currentPosition + movement);

    shootTarget_();
}

void FlyingEnemyAI::lateUpdate() {

}

void FlyingEnemyAI::updateNextPoint_() {
    lastPoint_ = nextPoint_;
    auto it = pathToTarget_.begin();
    if (it == pathToTarget_.end()) {
        return;
    }
    nextPoint_ = *it;
    pathToTarget_.erase(it);
}