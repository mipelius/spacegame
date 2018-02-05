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
    pathUpdateTimer_.setInterval(1000);
    pathUpdateTimer_.setIntervalRandomness(0);
    pathUpdateTimer_.reset();

    nextPoint_ = transform()->getPosition();
    lastPoint_ = nextPoint_;
}

void FlyingEnemyAI::update() {

    if (pathUpdateTimer_.resetIfTimeIntervalPassed()) {
        Rect boundingBox = gameObject()->getComponent<PolygonCollider>()->boundingBox();

        pathToTarget_ = Tile2D::pathFinder().getPath(
                transform()->getPosition(),
                target_->getPosition(),
                {
                        boundingBox.x1 + 8.0f,
                        boundingBox.y1 + 8.0f,
                        boundingBox.x2 - 8.0f,
                        boundingBox.y2 - 8.0f
                }
        );
        updateNextPoint_();
    }

    if ((transform()->getPosition() - nextPoint_).length() < 16.0f) {
        updateNextPoint_();
    }

    Vecf currentPosition = transform()->getPosition();
    Vecf movement = (nextPoint_ - lastPoint_) / 2;

    transform()->setPosition(currentPosition + movement);
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
