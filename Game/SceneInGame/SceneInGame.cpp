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


#include <Game/MusicManager.h>
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
#include "SceneManager.h"
#include "TeleportBehaviour.h"

void SceneInGame::init() {
    MusicManager::getInstance()->turnOn();

    // Scene setup: tile map, physics, light system
    Tile2D::tileMap().load(mapPath_, tileSetPath_);
    Tile2D::lightSystem().setAmbientLight(0.0f);
    Tile2D::lightSystem().setBlendedShadowsEnabled(false);
    Tile2D::physicsWorld().setAirDensity(0.2f);
    Tile2D::physicsWorld().setGForce({0, 100.0f});

    // player
    auto player = playerPrefab_->instantiate();
    //player->attachComponent<DebugBehaviour>();
    player->transform().setPosition({500.0f, 3600.0f});
    player->getComponent<Inventory>()->getItem(4)->setIsActivated(true);

    // boss
    auto boss = bossPrefab_->instantiate();
    boss->getComponent<EnemyAIBase>()->setTarget(player);
    boss->transform().setPosition({3264.0f, 12984.0f});
    boss->getComponent<Health>()->onDeath.add([] (Health* owner, GameObjectDiedEventArgs) {
        MusicManager::getInstance()->turnOff();
        Tile2D::executeDelayedFunction(owner->gameObject(), 2000, [] (GameObject* owner) {
            Tile2D::sceneManager().loadScene(Tile2D::sceneManager().getCount() - 1);
        });
    });

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
    auto windowSize = Tile2D::window().getSize();

    camera_ = new Camera;
    camera_->setAreaRect({0, 0, (float)windowSize.x, (float)windowSize.y});
    Tile2D::canvas().setCamera(camera_);

    // teleports
    auto teleport1 = createTeleport(
            player,
            {300.0f, 3600.0f}
    );
    teleport1->setIsActive(false);

    auto teleport2 = createTeleport(
            player,
            {5960.0f, 8912.0f}
    );

    teleport1->getComponent<TeleportBehaviour>()->setDestinationTeleport(teleport2);
    teleport2->getComponent<TeleportBehaviour>()->setDestinationTeleport(teleport1);
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
    bossPrefab_ = Tile2D::resources().prefabs[jsonObject["boss"].ToString()];

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

GameObject* SceneInGame::createTeleport(GameObject *player, const Vecf& position) {
    Rect spriteRect = {-250.0f, -250.0f, 250.0f, 250.0f};
    Rect teleportRect = {-100.0f, -100.0f, 100.0f, 100.0f};

    auto teleport = Tile2D::createGameObject();
    teleport->transform().setPosition(position);

    auto teleportSprite = teleport->attachComponent<Sprite>();
    teleportSprite->setRect(spriteRect);
    teleportSprite->setTexturePtr(Tile2D::resources().textures["teleport"]);

    auto teleportLight = teleport->attachComponent<PointLight>();
    teleportLight->setIntensity(1.0f);
    teleportLight->setRadius(300.0f);

    auto teleportBehaviour = teleport->attachComponent<TeleportBehaviour>();
    teleportBehaviour->setPlayer(player);
    teleportBehaviour->setRect(teleportRect);

//    auto teleportParticleSystem = teleport->attachComponent<ParticleSystem>();
//    teleportParticleSystem

    return teleport;
}
