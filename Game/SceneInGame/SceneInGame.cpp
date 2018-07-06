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


#include "JsonFileManager.h"
#include "ItemTags.h"
#include "DrawableMap.h"
#include "SceneInGame.h"
#include "GameObject.h"
#include "EnemyAIBase.h"
#include "TileMap.h"
#include "LightSystem.h"
#include "PhysicsWorld.h"
#include "Window.h"
#include "Prefab.h"
#include "Resources.h"
#include "PickupSpawner.h"
#include "DebugBehaviour.h"
#include "HUD.h"
#include "Tile2D.h"

void SceneInGame::init() {
    // Scene setup: tile map, physics, light system
    Tile2D::tileMap().load(mapPath_, tileSetPath_);
    Tile2D::lightSystem().setAmbientLight(0.0f);
    Tile2D::physicsWorld().setAirDensity(0.2f);
    Tile2D::physicsWorld().setGForce({0, 100.0f});

    // player
    auto player = playerPrefab_->instantiate();
    player->attachComponent<DebugBehaviour>();
    player->transform().setPosition({500.0f, 250.0f});

    // hud
    auto hudObject = Tile2D::createGameObject();
    auto hud = hudObject->attachComponent<HUD>();
    hud->setPlayer(player);

    // spawners
    for (auto spawnerPrefab : spawnerPrefabs_) {
        auto spawner = spawnerPrefab->instantiate();
        spawner->getComponent<SpawnerBase>()->setPlayer(player);
    }

    // other game objects
    for (auto prefabSetup : otherPrefabs_) {
        auto gameObject = prefabSetup.prefab->instantiate();
        gameObject->transform().setPosition(prefabSetup.position);
    }

    // camera
    camera_ = new Camera;
    camera_->setAreaRect({0, 0, (float)Tile2D::window().getW(), (float)Tile2D::window().getH()});
    Tile2D::canvas().setCamera(camera_);
}

void SceneInGame::destroy() {
    Tile2D::canvas().setCamera(nullptr);
    Tile2D::tileMap().unload();
    delete camera_;
}

void SceneInGame::deserialize(const json::Object &jsonObject) {
    auto tileMapJson = jsonObject["tilemap"];
    mapPath_ = tileMapJson["map"].ToString();
    tileSetPath_ = tileMapJson["tileset"].ToString();

    playerPrefab_ = Tile2D::resources().prefabs[jsonObject["player"].ToString()];

    for (const json::Value& spawnerJson : jsonObject["spawners"].ToArray()) {
        auto spawnerPrefab = Tile2D::resources().prefabs[spawnerJson.ToString()];
        spawnerPrefabs_.push_back(spawnerPrefab);
    }

    for (const json::Value& prefabSetupJson : jsonObject["other"].ToArray()) {
        std::string prefabName;
        Vecf position;

        if (prefabSetupJson.GetType() == json::ArrayVal) {
            auto prefabSetupJsonArray = prefabSetupJson.ToArray();
            prefabName = prefabSetupJsonArray[0].ToString();

            position = {
                    prefabSetupJsonArray[1].ToFloat(),
                    prefabSetupJsonArray[2].ToFloat()
            };
        }
        else {
            prefabName = prefabSetupJson.ToString();
        }

        otherPrefabs_.push_back(
                {
                        Tile2D::resources().prefabs[prefabName],
                        position
                }
        );
    }
}

