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
#include "BodyCollisionEventArgs.h"
#include "DrawableGroup.h"
#include "Background.h"
#include "Sprite.h"
#include "PlayerController.h"
#include "Tags.h"

static GameObject* spawnEnemy(Vecf pos) {
    auto enemy = Tile2D::createGameObject();
    enemy->transform().setPosition(pos);
    enemy->transform().setRotation(0.0f);

    enemy->tag = Tags::enemy;

    auto spaceshipBody = enemy->attachComponent<Body>();
    spaceshipBody->setMass(100.0);

    auto polygonCollider = enemy->attachComponent<PolygonCollider>();
    polygonCollider->setPoints({
                                       {-20, -18},
                                       {-5, -18},
                                       {18, 0},
                                       {-5, 18},
                                       {-20, 18}
                               });

    auto spaceshipSprite = enemy->attachComponent<Sprite>();
    spaceshipSprite->setRect({-20, -20, 20, 20});
    spaceshipSprite->setTexturePtr(Tile2D::resources().textures["spaceship"]);
    spaceshipSprite->setColor({1, 0, 0});

    return enemy;
}

void SceneInGame::init() {
    // Scene setup: tile map, physics, light system

    Tile2D::tileMap().load("maps/map.bmp", "json/tileset.json");
    Tile2D::lightSystem().setAmbientLight(0.0);
    Tile2D::physicsWorld().setAirDensity(0.0001);

    // background

    auto background = Tile2D::createGameObject();
    auto bg = background->attachComponent<Background>();
    bg->setRatio(0.5f);
    bg->setTexturePtr(Tile2D::resources().textures["bg2"]);

    // player

    auto player = Tile2D::createGameObject();
    player->transform().setPosition({500.0f, 250.0f});
    player->transform().setRotation(0.0f);

    player->tag = Tags::player;

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

    auto spaceshipSprite = player->attachComponent<Sprite>();
    spaceshipSprite->setRect({-20, -20, 20, 20});
    spaceshipSprite->setTexturePtr(Tile2D::resources().textures["spaceship"]);

    auto light = player->attachComponent<PointLight>();
    light->setIntensity(1.0);
    light->setRadius(400.0);

    auto playerController = player->attachComponent<PlayerController>();
    playerController->moveForce = 10000.0f;

    // camera

    camera = new Camera;
    camera->setAreaRect({0, 0, (float)Tile2D::window().getW(), (float)Tile2D::window().getH()});
    Tile2D::canvas().setCamera(camera);

    // dummy enemies

    spawnEnemy(player->transform().getPosition() + Vecf(100.0, 0.0));
    spawnEnemy(player->transform().getPosition() + Vecf(200.0, 0.0));
    spawnEnemy(player->transform().getPosition() + Vecf(300.0, 0.0));

    // light test
    auto lightObj = Tile2D::createGameObject();

    auto light2 = lightObj->attachComponent<PointLight>();
    light2->setRadius(500);

    auto light1 = lightObj->attachComponent<PointLight>();
    light1->setRadius(20);
}

void SceneInGame::destroy() {
    delete camera;
}
