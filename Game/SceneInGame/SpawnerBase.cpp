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
#include "SpawnerBase.h"
#include "Spawnable.h"
#include "GameObject.h"
#include "Resources.h"
#include "Prefab.h"

void SpawnerBase::onDestroy() {
	Tile2DBehaviour::onDestroy();
	for (auto obj : spawnedGameObjects_) {
		obj->getComponent<Spawnable>()->setSpawner(nullptr);
		obj->destroy();
	}
}

GameObject *SpawnerBase::getPlayer() const {
    return player_;
}

void SpawnerBase::setPlayer(GameObject *player) {
    player_ = player;
}

void SpawnerBase::remove(GameObject *gameObject) {
    spawnedGameObjects_.remove(gameObject);
}

void SpawnerBase::setPrefab(const std::string& prefabString) {
    prefabString_ = prefabString;
}

unsigned int SpawnerBase::getMaxSpawnedObjects() const {
    return maxSpawnedObjects_;
}

void SpawnerBase::setMaxSpawnedObjects(unsigned int maxSpawnedObjects) {
    maxSpawnedObjects_ = maxSpawnedObjects;
}

GameObject* SpawnerBase::spawn() {
    if (spawnedGameObjects_.size() >= maxSpawnedObjects_) {
        timer_.reset();
        return nullptr;
    }
    if (!timer_.timeIntervalPassed()) {
        return nullptr;
    }

    auto gameObject = Tile2D::resources().prefabs[prefabString_]->instantiate();
    auto spawnable = gameObject->attachComponent<Spawnable>();
    spawnable->setSpawner(this);
    spawnedGameObjects_.push_back(gameObject);
    timer_.reset();

    return gameObject;
}

void SpawnerBase::setSpawningDelay(Uint32 milliseconds) {
    timer_.setInterval(milliseconds);
}

const Rect &SpawnerBase::getAreaRect() const {
    return areaRect_;
}

void SpawnerBase::setAreaRect(const Rect &areaRect) {
    areaRect_ = areaRect;
}

void SpawnerBase::deserialize(const json::Object &jsonObject) {
    if (jsonObject.HasKey("prefab")) {
        setPrefab(jsonObject["prefab"].ToString());
    }
    if (jsonObject.HasKey("maxSpawnedObjects")) {
        maxSpawnedObjects_ = (unsigned int)jsonObject["maxSpawnedObjects"].ToInt();
    }
    if (jsonObject.HasKey("spawningDelay")) {
        setSpawningDelay((Uint32)jsonObject["spawningDelay"].ToInt());
    }
    if (jsonObject.HasKey("areaRect")) {
        auto areaRectJson = jsonObject["areaRect"].ToObject();
        areaRect_.deserialize(areaRectJson);
    }
}