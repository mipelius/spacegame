// This file is part of SpaceGame.
// Copyright (C) 2017 Miika Pelkonen
//
// SpaceGame is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// SpaceGame is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SpaceGame.  If not, see <http://www.gnu.org/licenses/>.

#include "DrawableMap.h"
#include "SceneInGame.h"
#include "DrawableGroup.h"
#include "Background.h"
#include "Sprite.h"
#include "PlayerController.h"
#include "Tags.h"
#include "BackgroundBehaviour.h"
#include "DebugBehaviour.h"
#include "ColliderLayers.h"
#include "PathFinder.h"
#include "Prefabs.h"
#include "SpawnerBehaviour.h"
#include "SortingLayers.h"

void createBackground(Rect area, const char* texture, Color color) {
    auto background = Tile2D::createGameObject();
    auto bg = background->attachComponent<Background>();
    bg->setRatio(0.5f);
    bg->setTexturePtr(Tile2D::resources().textures[texture]);
    bg->setColor(color);
    bg->setOpacity(0.0f);
    bg->setSortingLayer(SortingLayers::background);
    auto bgBehaviour = background->attachComponent<BackgroundBehaviour>();
    bgBehaviour->setArea(area);
}

void SceneInGame::init() {
    // Scene setup: tile map, physics, light system
    Tile2D::tileMap().load("maps/map.bmp", "json/tileset.json");
    Tile2D::lightSystem().setAmbientLight(0.5f);
    Tile2D::physicsWorld().setAirDensity(0.0001);

    // backgrounds
    createBackground({0.0f, 0.0f, 3300.0f, 2400.0f},       "bg2", {0.8f, 1.0f, 0.5f}); // upper left corner
    createBackground({3300.0f, 0.0f, 6400.0f, 2400.0f},    "bg2", {0.2f, 0.8f, 1.0f}); // upper right corner
    createBackground({0.0f, 2400.0f, 3300.0f, 4800.0f},    "bg1", {0.6f, 0.3f, 0.3f}); // bottom left corner
    createBackground({3300.0f, 2400.0f, 6400.0f, 4800.0f}, "bg1", {0.3f, 0.3f, 0.6f}); // bottom right corner

    // player
    auto player = Tile2D::createGameObject();
    player->transform().setPosition({500.0f, 250.0f});
    player->transform().setRotation(0.0f);

    player->tag = Tags::player;

    auto debugBehaviour = player->attachComponent<DebugBehaviour>();

    auto spaceshipBody = player->attachComponent<Body>();
    spaceshipBody->setMass(100.0);

    auto polygonCollider = player->attachComponent<PolygonCollider>();
    polygonCollider->setPoints({
                                       {-20, -18},
                                       {-5, -18},
                                       {18, 0},
                                       {-5, 18},
                                       {-20, 18}
                               });
    polygonCollider->setLayer(ColliderLayers::player);

    auto spaceshipSprite = player->attachComponent<Sprite>();
    spaceshipSprite->setSortingLayer(SortingLayers::player);
    spaceshipSprite->setRect({-30, -30, 30, 30});
    spaceshipSprite->setTexturePtr(Tile2D::resources().textures["spaceship_small"]);

    auto light = player->attachComponent<PointLight>();
    light->setIntensity(1.0);
    light->setRadius(500.0);

    auto playerController = player->attachComponent<PlayerController>();
    playerController->moveForce = 10000.0f;

    // camera
    camera = new Camera;
    camera->setAreaRect({0, 0, (float)Tile2D::window().getW(), (float)Tile2D::window().getH()});
    Tile2D::canvas().setCamera(camera);

    // spawner
    GameObject* spawner = Tile2D::createGameObject();
    auto spawnerBehaviour = spawner->attachComponent<SpawnerBehaviour>();
    spawnerBehaviour->setTarget(player);
    spawnerBehaviour->setInnerRect({-600.0f, -400.0f, 600.0f, 400.0f});
    spawnerBehaviour->setOuterRect({-1000.0f, -800.0f, 1000.0f, 800.0f});

    // healthbar (test)
    GameObject* healthBar = Tile2D::createGameObject();
    healthBar->transform().setPosition({20.0f, 20.0f});

    auto healthBarSprite = healthBar->attachComponent<Sprite>();
    healthBarSprite->setRect({0.0f, -5.0f, 300.0f, 5.0f});
    healthBarSprite->setColor({1.0f, 0.0f, 0.0f});
    healthBarSprite->setIsUIDrawable(true);
}

void SceneInGame::destroy() {
    Tile2D::canvas().setCamera(nullptr);
    Tile2D::tileMap().unload();
    delete camera;
}
