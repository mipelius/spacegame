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

#include "Tile2D.h"
#include "PhysicsWorld.h"
#include "Body.h"
#include "Prefab.h"
#include "Resources.h"
#include "BombDropper.h"
#include "GameObject.h"

void BombDropper::shoot(const Vecf &from, const Vecf &direction, const Vecf &shooterVelocity) {
    auto bomb = bombPrefab_->instantiate();
    bomb->transform().setPosition(from);
    bomb->getComponent<Body>()->setVelocity(shooterVelocity / 2 + Vecf(0, 100));

    auto bombCollider = bomb->getComponent<PolygonCollider>();
    bombCollider->setLayer(Tile2D::physicsWorld().getColliderLayerMatrix().getColliderLayer(0));
}

ItemBase *BombDropper::clone() {
    return new BombDropper(*this);
}

void BombDropper::deserialize(const json::Object &jsonObject) {
    WeaponBase::deserialize(jsonObject);

    if (jsonObject.HasKey("bombPrefab")) {
        auto bombPrefabName = jsonObject["bombPrefab"].ToString();
        bombPrefab_ = Tile2D::resources().prefabs[bombPrefabName];
    }
}

Prefab *BombDropper::getBombPrefab_() const {
    return bombPrefab_;
}

void BombDropper::setBombPrefab_(Prefab *bombPrefab_) {
    BombDropper::bombPrefab_ = bombPrefab_;
}
