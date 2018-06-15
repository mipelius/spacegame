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


#include "EnemyAIBase.h"

#include "Tile2D.h"
#include "Prefabs.h"
#include "TileMap.h"
#include "Reflector.h"

void EnemyAIBase::awake() {
    body_ = gameObject()->getComponent<Body>();
}

Transform *EnemyAIBase::getTarget() const {
    return target_;
}

void EnemyAIBase::setTarget(Transform *target) {
    target_ = target;
}

void EnemyAIBase::shootTarget_() {
    if (weapon_ != nullptr && canSeeTarget_()) {
        weapon_->use(gameObject());
    }
}

void EnemyAIBase::update() {
    float distanceSqr = (target_->getPosition() - transform()->getPosition()).lengthSqr();

    if (distanceSqr > (maxDistance_ * maxDistance_)) {
        gameObject()->destroy();
    }
}

float EnemyAIBase::getMaxDistance() const {
    return maxDistance_;
}

void EnemyAIBase::setMaxDistance(float maxDistance) {
    maxDistance_ = maxDistance;
}

bool EnemyAIBase::canSeeTarget_() {
    Vecf collisionPoint;
    return !Tile2D::tileMap().castLine(
            transform()->getPosition(),
            target_->getPosition(),
            collisionPoint
    );
}

void EnemyAIBase::deserialize(const json::Object &jsonObject) {
    if (jsonObject.HasKey("maxDistance")) {
        maxDistance_ = jsonObject["maxDistance"].ToFloat();
    }
    if (jsonObject.HasKey("weapon")) {
        auto weaponJson = jsonObject["weapon"];
        auto className = weaponJson["class"].ToString();
        auto propertiesJson = weaponJson["properties"].ToObject();

        auto weapon = Tile2D::reflector().instantiate(className);
        weapon->deserialize(propertiesJson);

        weapon_ = (WeaponBase*)weapon;
    }
}

EnemyAIBase::~EnemyAIBase() {
    delete weapon_;
}