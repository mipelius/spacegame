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
#include "Plot.h"
#include "Background.h"
#include "Sprite.h"
#include "PlayerController.h"
#include "Tags.h"

static GameObject* spawnEnemy(Vec pos) {
    auto enemy = Tile2D::createGameObject();
    enemy->tag = Tags::enemy;

    auto spaceshipBody = enemy->attachComponent<Body>();
    spaceshipBody->mass.set(100.0);
    spaceshipBody->position.set(pos);

    auto polygonCollider = enemy->attachComponent<PolygonCollider>();
    polygonCollider->setPoints({
                                       {-20, -18},
                                       {-5, -18},
                                       {18, 0},
                                       {-5, 18},
                                       {-20, 18}
                               });

    auto spaceshipSprite = enemy->attachComponent<Sprite>();
    spaceshipSprite->position.set({0, 0});
    spaceshipSprite->rect.set({-20, -20, 20, 20});
    spaceshipSprite->texturePtr.set(Tile2D::resources().textures["spaceship"]);
    spaceshipSprite->position.bind(spaceshipBody->position);
    spaceshipSprite->angle.bind(spaceshipBody->angle);
    spaceshipSprite->color.set({1, 0, 0});
//
//    auto light = enemy->attachComponent<PointLight>();
//    light->position.bind(spaceshipBody->position);
//    light->intensity.set(1.0);
//    light->radius.set(100.0);

    return enemy;
}

void SceneInGame::init() {
    // Scene setup: tile map, physics, light system

    Tile2D::tileMap().load("maps/map.bmp", "json/tileset.json");
    Tile2D::lightSystem().ambientLight.set(0.0);
    Tile2D::physicsWorld().airDensity.set(0.0001);

    // background

    auto background = Tile2D::createGameObject();
    auto bg = background->attachComponent<Background>();
    bg->ratio.set(0.5f);
    bg->texturePtr.set(Tile2D::resources().textures["bg2"]);

    // player

    auto player = Tile2D::createGameObject();
    player->tag = Tags::player;

    auto spaceshipBody = player->attachComponent<Body>();
    spaceshipBody->mass.set(100.0);
    spaceshipBody->position.set({500.0, 250.0});

    auto polygonCollider = player->attachComponent<PolygonCollider>();
    polygonCollider->setPoints({
                                       {-20, -18},
                                       {-5, -18},
                                       {18, 0},
                                       {-5, 18},
                                       {-20, 18}
                               });

    auto spaceshipSprite = player->attachComponent<Sprite>();
    spaceshipSprite->position.set({0, 0});
    spaceshipSprite->rect.set({-20, -20, 20, 20});
    spaceshipSprite->texturePtr.set(Tile2D::resources().textures["spaceship"]);
    spaceshipSprite->position.bind(spaceshipBody->position);
    spaceshipSprite->angle.bind(spaceshipBody->angle);

    auto light = player->attachComponent<PointLight>();
    light->position.bind(spaceshipBody->position);
    light->intensity.set(1.0);
    light->radius.set(500.0);

    auto playerController = player->attachComponent<PlayerController>();
    playerController->moveForce = 10000.0f;

    // camera

    camera = new Camera;
    camera->areaRect.set({0, 0, (double)Tile2D::window().w.get(), (double)Tile2D::window().h.get()});
    Tile2D::canvas().setCamera(camera);

    // dummy enemies

    spawnEnemy(spaceshipBody->position.get() + Vec(100.0, 0.0));
    spawnEnemy(spaceshipBody->position.get() + Vec(200.0, 0.0));
    spawnEnemy(spaceshipBody->position.get() + Vec(300.0, 0.0));

    // light test
    auto lightObj = Tile2D::createGameObject();

    auto light2 = lightObj->attachComponent<PointLight>();
    light2->radius.set(500);
    light2->position.set({500, 500});

    auto light1 = lightObj->attachComponent<PointLight>();
    light1->radius.set(20);
    light1->position.set({500, 500});

}

void SceneInGame::destroy() {
    delete camera;
}
