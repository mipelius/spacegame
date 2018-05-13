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

#include "Inventory.h"
#include "Tile2D.h"
#include "TileMap.h"
#include "Tile2DMath.h"
#include "PickupSpawner.h"

void PickupSpawner::awake() { }

void PickupSpawner::update() {
    if (player_ == nullptr || itemTag_ == -1) {
        return;
    }

    auto item = player_->getComponent<Inventory>()->getItem(itemTag_);
    if (item->getCount() >= maxItemCount_) {
        return;
    }

    auto object = spawn();
    if (object == nullptr) {
        return;
    }

    const auto& boundingBox = object->getComponent<PolygonCollider>()->boundingBox();
    Vecf position = {};

    do {
        position = {
            Mathf::random(areaRect_.x1, areaRect_.x2),
            Mathf::random(areaRect_.y1, areaRect_.y2)
        };
        if ((position - player_->transform().getPosition()).length() < minDistanceToTarget_) {
            continue;
        }
    } while(!Tile2D::tileMap().canMoveScaled(position, boundingBox));

    object->transform().setPosition(position);
}

void PickupSpawner::lateUpdate() { }

float PickupSpawner::getMinDistanceToTarget() const {
    return minDistanceToTarget_;
}

void PickupSpawner::setMinDistanceToTarget(float minDistanceToTarget) {
    minDistanceToTarget_ = minDistanceToTarget;
}

int PickupSpawner::getMaxItemCount() const {
    return maxItemCount_;
}

void PickupSpawner::setMaxItemCount(int maxItemCount) {
    maxItemCount_ = maxItemCount;
}

int PickupSpawner::getItemTag() const {
    return itemTag_;
}

void PickupSpawner::setItemTag(int itemTag) {
    itemTag_ = itemTag;
}
