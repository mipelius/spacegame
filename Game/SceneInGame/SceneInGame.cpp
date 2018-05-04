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


#include "DrawableMap.h"
#include "SceneInGame.h"
#include "Sprite.h"
#include "Prefabs.h"
#include "SpawnerBehaviour.h"
#include "EnemyAIBase.h"
#include "HUD.h"
#include "TileMap.h"
#include "LightSystem.h"
#include "PhysicsWorld.h"
#include "Window.h"

void SceneInGame::init() {
    // Scene setup: tile map, physics, light system
    Tile2D::tileMap().load("data/maps/map.bmp", "data/maps/tileset.json");
    Tile2D::lightSystem().setAmbientLight(0.0f);
    Tile2D::physicsWorld().setAirDensity(0.2f);
    Tile2D::physicsWorld().setGForce({0, 100.0f});

    // backgrounds
    Prefabs::background({0.0f, 0.0f, 3300.0f, 2400.0f},       "bg2", {0.8f, 0.8f, 0.5f}); // upper left corner
    Prefabs::background({3300.0f, 0.0f, 6400.0f, 2400.0f},    "bg2", {0.2f, 0.8f, 1.0f}); // upper right corner
    Prefabs::background({0.0f, 2400.0f, 3300.0f, 4800.0f},    "bg1", {0.6f, 0.3f, 0.3f}); // bottom left corner
    Prefabs::background({3300.0f, 2400.0f, 6400.0f, 4800.0f}, "bg1", {0.3f, 0.3f, 0.6f}); // bottom right corner

    // player
    auto player = Prefabs::player();
    player->transform().setPosition({500.0f, 250.0f});

    // hud
    auto hud = Prefabs::hud(player);

    // camera
    camera_ = new Camera;
    camera_->setAreaRect({0, 0, (float)Tile2D::window().getW(), (float)Tile2D::window().getH()});
    Tile2D::canvas().setCamera(camera_);

    // spawners
    Prefabs::spawner({0.0f, 0.0f, 3300.0f, 2400.0f}, player, Prefabs::walker);
    Prefabs::spawner({3300.0f, 0.0f, 6400.0f, 2400.0f}, player, Prefabs::wurm);
    Prefabs::spawner({3300.0f, 2400.0f, 6400.0f, 4800.0f}, player, Prefabs::evilEye);
    Prefabs::spawner({0.0f, 2400.0f, 3300.0f, 4800.0f}, player, Prefabs::walker);

    // boss
    auto boss = Prefabs::boss();
    boss->transform().setPosition({2622.0f ,3880.0f});
    boss->getComponent<EnemyAIBase>()->setTarget(&player->transform());

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

    for (auto lightPosition : lightPositions) {
        auto light = Prefabs::light();
        light->transform().setPosition(lightPosition);
    }
}

void SceneInGame::destroy() {
    Tile2D::canvas().setCamera(nullptr);
    Tile2D::tileMap().unload();
    delete camera_;
}
