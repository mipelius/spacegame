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
    Tile2D::tileMap().load("data/maps/map.bmp", "data/maps/tileset.json");
    Tile2D::lightSystem().setAmbientLight(0.0f);
    Tile2D::physicsWorld().setAirDensity(0.2f);
    Tile2D::physicsWorld().setGForce({0, 100.0f});

    // player
    auto player = Tile2D::resources().prefabs["player"]->instantiate();
    player->attachComponent<DebugBehaviour>();
    player->transform().setPosition({500.0f, 250.0f});

    // hud
    auto hudObject = Tile2D::createGameObject();
    auto hud = hudObject->attachComponent<HUD>();
    hud->setPlayer(player);

    // worlds
    initSnowWorld_(player);
    initGreenWorld_(player);
    initBlueWorld_(player);
    initRedWorld_(player);

    // camera
    camera_ = new Camera;
    camera_->setAreaRect({0, 0, (float)Tile2D::window().getW(), (float)Tile2D::window().getH()});
    Tile2D::canvas().setCamera(camera_);

    // pickup spawners
    auto pickupSpawner = Tile2D::resources().prefabs["pickup_spawner_bomb"]->instantiate();
    pickupSpawner->getComponent<PickupSpawner>()->setPlayer(player);

    // lights
    std::vector<Vecf> lightPositions = {
            {421.042f, 1516.95f},
            {804.303f, 1462.63f},
            {1375.13f, 825.478f},
            {1709.78f, 1018.84f},
            {1950.89f, 746.274f},
            {2469.61f, 1363.59f},
            {2688.58f, 1981.85f},
            {2964.39f, 1436.52f},
            {3388.94f, 439.624f},
            {4022.24f, 1002.53f},
            {3933.65f, 1367.12f},
            {4313.51f, 1671.43f},
            {5038.69f, 402.691f},
            {5405.34f, 533.581f},
            {6147.14f, 847.582f},
            {5899.84f, 1281.75f},
            {5449.54f, 1662.89f},
            {5395.56f, 2340.16f},
            {5849.67f, 3617.31f},
            {5399.64f, 3871.27f},
            {4795.65f, 3539.41f},
            {4315.63f, 2931.38f},
            {3950.27f, 3030.98f},
            {4169.71f, 4031.18f},
            {3696.42f, 4214.83f},
            {2801.86f, 4414.57f},
            {1581.64f, 4237.54f},
            {1161.97f, 4532.58f},
            {492.366f, 3946.02f},
            {1031.4f, 3113.18f},
            {235.148f, 635.856f},
            {1016.77f, 707.95f},
            {2857.8f, 544.041f},
            {2383.11f, 4031.93f},
            {2431.36f, 4086.46f},
            {2180.14f, 3076.49f},
            {1844.56f, 2904.61f},
            {1045.9f, 2757.18f},
            {494.834f, 3273.01f},
            {1139.6f, 3061.95f},
            {1093.01f, 3158.79f},
            {2100.41f, 4450.53f}
    };

    for (const auto& lightPosition : lightPositions) {
        auto light = Tile2D::resources().prefabs["fx_light"]->instantiate();
        light->transform().setPosition(lightPosition);
    }

    auto box1 = Tile2D::resources().prefabs["pickup_gatling"]->instantiate();
    box1->transform().setPosition({200.0f, 3300.0f});

    auto box2 = Tile2D::resources().prefabs["pickup_plasma_cannon"]->instantiate();
    box2->transform().setPosition({300.0f, 3300.0f});

    auto box3 = Tile2D::resources().prefabs["pickup_bomb"]->instantiate();
    box3->transform().setPosition({400.0f, 3300.0f});
    auto box4 = Tile2D::resources().prefabs["pickup_bomb"]->instantiate();
    box4->transform().setPosition({500.0f, 3300.0f});

    auto box5 = Tile2D::resources().prefabs["pickup_medikit"]->instantiate();
    box5->transform().setPosition({600.0f, 3300.0f});
    auto box6 = Tile2D::resources().prefabs["pickup_medikit"]->instantiate();
    box6->transform().setPosition({700.0f, 3300.0f});

    auto box7 = Tile2D::resources().prefabs["pickup_laser_cannon_upgrade"]->instantiate();
    box7->transform().setPosition({800.0f, 3300.0f});

    auto box8 = Tile2D::resources().prefabs["pickup_health_upgrade"]->instantiate();
    box8->transform().setPosition({900.0f, 3300.0f});
    auto box9 = Tile2D::resources().prefabs["pickup_health_upgrade"]->instantiate();
    box9->transform().setPosition({1000.0f, 3300.0f});

    auto box10 = Tile2D::resources().prefabs["pickup_power_upgrade"]->instantiate();
    box10->transform().setPosition({1100.0f, 3300.0f});
    auto box11 = Tile2D::resources().prefabs["pickup_power_upgrade"]->instantiate();
    box11->transform().setPosition({1200.0f, 3300.0f});
}

void SceneInGame::destroy() {
    Tile2D::canvas().setCamera(nullptr);
    Tile2D::tileMap().unload();
    delete camera_;
}


void SceneInGame::initSnowWorld_(GameObject* player) {
    auto spawner = Tile2D::resources().prefabs["enemy_spawner_fish"]->instantiate();
    spawner->getComponent<EnemySpawner>()->setPlayer(player);
    Tile2D::resources().prefabs["bg_snow"]->instantiate();
}

void SceneInGame::initGreenWorld_(GameObject* player) {
    auto spawner = Tile2D::resources().prefabs["enemy_spawner_walker"]->instantiate();
    spawner->getComponent<EnemySpawner>()->setPlayer(player);
    Tile2D::resources().prefabs["bg_green"]->instantiate();
}

void SceneInGame::initBlueWorld_(GameObject* player) {
    auto spawner = Tile2D::resources().prefabs["enemy_spawner_trifly"]->instantiate();
    spawner->getComponent<EnemySpawner>()->setPlayer(player);
    Tile2D::resources().prefabs["bg_blue"]->instantiate();
}

void SceneInGame::initRedWorld_(GameObject* player) {
    auto spawner = Tile2D::resources().prefabs["enemy_spawner_wanderer"]->instantiate();
    spawner->getComponent<EnemySpawner>()->setPlayer(player);
    Tile2D::resources().prefabs["bg_red"]->instantiate();

    // boss
    auto boss = Tile2D::resources().prefabs["enemy_boss"]->instantiate();
    boss->transform().setPosition({3016.0f, 13088.0f});
    boss->getComponent<EnemyAIBase>()->setTarget(&player->transform());
}

void SceneInGame::deserialize(const json::Object &jsonObject) {

}

