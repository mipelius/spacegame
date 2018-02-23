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
    Tile2D::lightSystem().setAmbientLight(0.5f);
    Tile2D::physicsWorld().setAirDensity(0.2f);
    Tile2D::physicsWorld().setGForce({0, 100.0f});

    // backgrounds
    Prefabs::background({0.0f, 0.0f, 3300.0f, 2400.0f},       "bg2", {0.8f, 1.0f, 0.5f}); // upper left corner
    Prefabs::background({3300.0f, 0.0f, 6400.0f, 2400.0f},    "bg2", {0.2f, 0.8f, 1.0f}); // upper right corner
    Prefabs::background({0.0f, 2400.0f, 3300.0f, 4800.0f},    "bg1", {0.6f, 0.3f, 0.3f}); // bottom left corner
    Prefabs::background({3300.0f, 2400.0f, 6400.0f, 4800.0f}, "bg1", {0.3f, 0.3f, 0.6f}); // bottom right corner

    // healthbar
    GameObject* healthBar = Tile2D::createGameObject();
    healthBar->transform().setPosition({20.0f, 20.0f});

    auto healthBarSprite = healthBar->attachComponent<Sprite>();
    healthBarSprite->setRect({0.0f, -5.0f, 0.0f, 5.0f});
    healthBarSprite->setColor({1.0f, 0.0f, 0.0f});
    healthBarSprite->setIsUIDrawable(true);

    // player
    auto player = Prefabs::player();
    player->transform().setPosition({500.0f, 250.0f});
    player->getComponent<HUD>()->setHealthSprite(healthBarSprite);

    // camera
    camera_ = new Camera;
    camera_->setAreaRect({0, 0, (float)Tile2D::window().getW(), (float)Tile2D::window().getH()});
    Tile2D::canvas().setCamera(camera_);

    // spawners
    Prefabs::spawner({0.0f, 0.0f, 3300.0f, 2400.0f}, player, Prefabs::walkingEnemy);
    Prefabs::spawner({3300.0f, 0.0f, 6400.0f, 2400.0f}, player, Prefabs::crabKindOf);
    Prefabs::spawner({0.0f, 2400.0f, 6400.0f, 4800.0f}, player, Prefabs::fourwayCyclops);

    // boss
    auto boss = Prefabs::boss();
    boss->transform().setPosition({2622.0f ,3880.0f});
    boss->getComponent<EnemyAIBase>()->setTarget(&player->transform());
}

void SceneInGame::destroy() {
    Tile2D::canvas().setCamera(nullptr);
    Tile2D::tileMap().unload();
    delete camera_;
}
