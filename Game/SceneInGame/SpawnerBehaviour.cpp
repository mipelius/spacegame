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


#include "SpawnerBehaviour.h"
#include "EnemyAIBase.h"
#include "Prefabs.h"
#include "Tile2D.h"
#include "Tile2DMath.h"

SpawnerBehaviour::SpawnerBehaviour() :
        outerRect_({0.0f, 0.0f, 0.0f, 0.0f}),
        innerRect_({0.0f, 0.0f, 0.0f, 0.0f})
{ }

void SpawnerBehaviour::awake() {
    spawningTimer_.setInterval(2000);
}

void SpawnerBehaviour::update() {

}

void SpawnerBehaviour::lateUpdate() {
    if (spawningTimer_.resetIfTimeIntervalPassed()) {
        Vecf targetVelocityDirection = target_->getComponent<Body>()->getVelocity().normalized();

        auto enemy = Prefabs::fourwayCyclops();
        auto boundingBox = enemy->getComponent<PolygonCollider>()->boundingBox();

        Rect rect       = {0.0f, 0.0f, 0.0f, 0.0f};
        Vecf position   = {0.0f, 0.0f};

        do {
            int rectCase = Mathi::random(0, 4); // cases 0 - 3

            switch(rectCase) {
                case 0 :        // TOP RECT
                    rect = {innerRect_.x1, outerRect_.y1, innerRect_.x2, innerRect_.y1};
                    break;
                case 1 :        // LEFT RECT
                    rect = {outerRect_.x1, outerRect_.y1, innerRect_.x1, outerRect_.y2};
                    break;
                case 2 :        // BOTTOM RECT
                    rect = {innerRect_.x1, innerRect_.y2, innerRect_.x2, outerRect_.y2};
                    break;
                case 3 :        // RIGHT RECT
                    rect = {innerRect_.x2, outerRect_.y1, outerRect_.x2, outerRect_.y2};
                    break;
                default: { }
            }

            Vecf offset = {
                    Mathf::random(rect.x1, rect.x2),
                    Mathf::random(rect.y1, rect.y2)
            };

            position = target_->transform().getPosition() + offset + (targetVelocityDirection * 200);
        } while(!Tile2D::tileMap().canMoveScaled(position, boundingBox));

        enemy->transform().setPosition(position);

        enemy->getComponent<EnemyAIBase>()->setTarget(&target_->transform());
    }
}

GameObject *SpawnerBehaviour::getTarget() const {
    return target_;
}

void SpawnerBehaviour::setTarget(GameObject *target) {
    target_ = target;
}

const Rect &SpawnerBehaviour::getOuterRect() const {
    return outerRect_;
}

void SpawnerBehaviour::setOuterRect(const Rect &outerRect) {
    outerRect_ = outerRect;
}

const Rect &SpawnerBehaviour::getInnerRect() const {
    return innerRect_;
}

void SpawnerBehaviour::setInnerRect(const Rect &innerRect) {
    innerRect_ = innerRect;
}
