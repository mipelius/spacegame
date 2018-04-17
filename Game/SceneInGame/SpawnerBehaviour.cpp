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



#include "SpawnerBehaviour.h"
#include "EnemyAIBase.h"
#include "Prefabs.h"
#include "Tile2D.h"
#include "Tile2DMath.h"
#include "TileMap.h"

SpawnerBehaviour::SpawnerBehaviour() :
        outerRect_({0.0f, 0.0f, 0.0f, 0.0f}),
        innerRect_({0.0f, 0.0f, 0.0f, 0.0f}),
        areaRect_({0.0f, 0.0f, 0.0f, 0.0f})
{ }

void SpawnerBehaviour::onDestroy() {
	Tile2DBehaviour::onDestroy();
	for (auto obj : spawnedGameObjects_) {
		obj->getComponent<EnemyAIBase>()->setSpawnerBehaviour(nullptr);
		obj->destroy();
	}
}

void SpawnerBehaviour::awake() {
    spawningTimer_.setInterval(500);
}

void SpawnerBehaviour::update() {

}

void SpawnerBehaviour::lateUpdate() {
    if (
            spawnedGameObjects_.size() >= maxSpawnedObjects_ ||
            !areaRect_.hasPointInside(target_->transform().getPosition())
    ) {
        return;
    }
    if (spawningTimer_.resetIfTimeIntervalPassed()) {
        Vecf targetVelocityDirection = target_->getComponent<Body>()->getVelocity().normalized();

        auto enemy = spawnFunction_();

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

        auto enemyAI = enemy->getComponent<EnemyAIBase>();

        if (enemyAI == nullptr) {
            throw std::runtime_error(
                    "Invalid spawn function: spawned object doesn't have component EnemyAIBase"
            );
        }

        enemyAI->setTarget(&target_->transform());
        enemyAI->setSpawnerBehaviour(this);

        spawnedGameObjects_.push_back(enemy);
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

void SpawnerBehaviour::remove(GameObject *gameObject) {
    spawnedGameObjects_.remove(gameObject);
}

void SpawnerBehaviour::setSpawnFunction(GameObject* (*spawnFunction)()) {
    spawnFunction_ = spawnFunction;
}

const Rect &SpawnerBehaviour::getAreaRect() const {
    return areaRect_;
}

void SpawnerBehaviour::setAreaRect(const Rect &areaRect) {
    areaRect_ = areaRect;
}

unsigned int SpawnerBehaviour::getMaxSpawnedObjects() const {
    return maxSpawnedObjects_;
}

void SpawnerBehaviour::setMaxSpawnedObjects(unsigned int maxSpawnedObjects) {
    maxSpawnedObjects_ = maxSpawnedObjects;
}
