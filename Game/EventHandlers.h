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

#ifndef SPACEGAME_EVENTHANDLERS_H
#define SPACEGAME_EVENTHANDLERS_H

#include "Event.h"
#include "Health.h"
#include "Prefabs.h"
#include "Tile2D.h"
#include "TileMap.h"
#include "CollisionEffects.h"
#include "t2Time.h"

class CollisionDamageHandlerBase :
        public IEventHandler<PolygonCollider, CollisionEventArgs>,
        public ISerializable
{

protected:
    const Tag* targetTag_ = nullptr;
    int damage_ = 0;
    virtual void damage(Health* targetHealth, PolygonCollider* owner, CollisionEventArgs args) const = 0;

public:
    void handle(PolygonCollider* owner, CollisionEventArgs args) const override {
        auto otherGameObject = args.otherCollider->gameObject();

        if (&otherGameObject->getTag() == targetTag_) {
            auto health = otherGameObject->getComponent<Health>();
            if (health == nullptr) {
                throw std::runtime_error("CollisionDamageHandlerBase: target has no health component!");
            }
            damage(health, owner, args);
        }
    }

    void deserialize(const json::Object &jsonObject) override {
        if (jsonObject.HasKey("damage")) {
            damage_ = jsonObject["damage"].ToInt();
        }
        if (jsonObject.HasKey("targetTag")) {
            int tagId = jsonObject["targetTag"].ToInt();
            targetTag_ = &Tile2D::getTag(tagId);
        }
    }
};

class ContinuousCollisionDamageHandler : public CollisionDamageHandlerBase {
protected:
    void damage(Health *targetHealth, PolygonCollider *owner, CollisionEventArgs args) const override {
        targetHealth->damage((int)(damage_ * Tile2D::time().getDeltaTime()), owner->gameObject());
        CollisionEffects::sparkles(owner->transform()->getPosition(), args.contactNormal, {1, 0, 0});
    }

    IEventHandler<PolygonCollider, CollisionEventArgs> *clone() override {
        return new ContinuousCollisionDamageHandler(*this);
    }
};

class AmmoCollisionDamageHandler : public CollisionDamageHandlerBase {
public:
    void damage(Health *targetHealth, PolygonCollider *owner, CollisionEventArgs args) const override {
        targetHealth->damage(damage_, owner->gameObject());
        owner->gameObject()->destroy();

        CollisionEffects::sparkles(owner->transform()->getPosition(), args.contactNormal, {1, 0, 0});
        CollisionEffects::pulseLight(owner->transform()->getPosition());
    }

    IEventHandler<PolygonCollider, CollisionEventArgs> *clone() override {
        return new AmmoCollisionDamageHandler(*this);
    }
};

class AmmoTerrainCollisionHandler :
        public IEventHandler<PolygonCollider, TerrainCollisionEventArgs>,
        public ISerializable
{

public:
    void handle(PolygonCollider* owner, TerrainCollisionEventArgs args) const override {
        owner->gameObject()->destroy();
        Tile2D::tileMap().setValueScaled(args.tileCoordinates, Tile2D::tileMap().getTileSet()->getEmptyBlock());
        CollisionEffects::sparkles(args.tileCoordinates, args.contactNormal, {1, 1, 1});
        CollisionEffects::pulseLight(owner->transform()->getPosition());
    }

    void deserialize(const json::Object &jsonObject) override { }

    IEventHandler<PolygonCollider, TerrainCollisionEventArgs> *clone() override {
        return new AmmoTerrainCollisionHandler(*this);
    }
};

class DeathHandler :
        public IEventHandler<Health, GameObjectDiedEventArgs>,
        public ISerializable
{
public:
    void handle(Health* health, GameObjectDiedEventArgs args) const override {
        health->gameObject()->destroy();
        auto bloodBurst = Prefabs::bloodBurst();
        bloodBurst->transform().setPosition(health->transform()->getPosition());
    }

    void deserialize(const json::Object &jsonObject) override { }

    IEventHandler<Health, GameObjectDiedEventArgs>* clone() override {
        return new DeathHandler(*this);
    };
};

#endif //SPACEGAME_EVENTHANDLERS_H
