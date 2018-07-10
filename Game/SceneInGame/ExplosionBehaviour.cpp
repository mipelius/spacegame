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

#include "Tile2DMath.h"
#include "PolygonCollider.h"
#include "Tile2D.h"
#include "TileMap.h"
#include "ExplosionBehaviour.h"
#include "GameObject.h"
#include "Health.h"
#include "LimitedLifetimeBehaviour.h"
#include "PhysicsWorld.h"

void ExplosionBehaviour::deserialize(const json::Object &jsonObject) {
    if (jsonObject.HasKey("terrainRemoveRange")) {
        terrainRemoveRange_ = jsonObject["terrainRemoveRange"].ToFloat();
    }
    if (jsonObject.HasKey("damageRange")) {
        damageRange_ = jsonObject["damageRange"].ToFloat();
    }
    if (jsonObject.HasKey("maxDamage")) {
        maxDamage_ = jsonObject["maxDamage"].ToFloat();
    }
}

Tile2DComponent *ExplosionBehaviour::clone() {
    return new ExplosionBehaviour(*this);
}

void ExplosionBehaviour::awake() {
    auto tileW = Tile2D::tileMap().getTileSet()->getTileW();
    auto tileH = Tile2D::tileMap().getTileSet()->getTileH();

    auto radiusX = (int)(terrainRemoveRange_ / tileW);
    auto radiusY = (int)(terrainRemoveRange_ / tileH);

    for (auto x=-radiusX; x<radiusX; ++x) {
        for (auto y=-radiusY; y<radiusY; ++y) {
            Vecf offset = {
                    (float)(x * tileW),
                    (float)(y * tileH)
            };
            if (offset.length() > terrainRemoveRange_) {
                continue;
            }
            Tile2D::tileMap().setValueScaled(
                    transform()->getPosition() + offset,
                    Tile2D::tileMap().getTileSet()->getEmptyBlock()
            );
        }
    }

    auto damager = Tile2D::createGameObject();
    damager->transform().setPosition(transform()->getPosition());

    auto damagerCollider = damager->attachComponent<PolygonCollider>();
    damagerCollider->setPoints({
                                       {-damageRange_, -damageRange_},
                                       {damageRange_,  -damageRange_},
                                       {damageRange_,  damageRange_},
                                       {-damageRange_, damageRange_}
                               });

    auto collisionHandler = new DamageFieldCollisionHandler();
    collisionHandler->maxDamage = maxDamage_;
    collisionHandler->range = damageRange_;

    damagerCollider->collision.add(collisionHandler);

    damagerCollider->setLayer(Tile2D::physicsWorld().getColliderLayerMatrix().getColliderLayer(5));
    damagerCollider->setResolverEnabled(false);

    auto limitedLifetimeBehaviour = damager->attachComponent<LimitedLifetimeBehaviour>();
    limitedLifetimeBehaviour->setTimeToLive(30);
}

void ExplosionBehaviour::update() { }

void ExplosionBehaviour::lateUpdate() { }

void ExplosionBehaviour::DamageFieldCollisionHandler::handle(PolygonCollider *owner, CollisionEventArgs args) const {
    auto otherGameObject = args.otherCollider->gameObject();

    auto health = otherGameObject->getComponent<Health>();
    if (health) {
        auto distance = (otherGameObject->transform().getPosition() - owner->transform()->getPosition()).length();
        auto damageCoefficient = (range - distance) / range;
        Mathf::clamp(damageCoefficient, 0, 1);
        damageCoefficient = sqrt(damageCoefficient);
        health->damage(damageCoefficient * maxDamage, owner->gameObject());
    }

    owner->gameObject()->destroy();
}

IEventHandler<PolygonCollider, CollisionEventArgs> *ExplosionBehaviour::DamageFieldCollisionHandler::clone() {
    return new DamageFieldCollisionHandler(*this);
}
