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
#include "Camera.h"
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

    return enemy;
}

void SceneInGame::init() {
    Tile2D::shadowMask().ambientLight.set(0.0);
    Tile2D::physicsWorld().airDensity.set(0.0001);

    // background

    auto background = Tile2D::createGameObject();
    auto bg = background->attachComponent<Background>();
    bg->ratio.set(0.5f);
    bg->texturePtr.set(Tile2D::resources().textures["bg2"]);

    // tile map

    auto tileMap = Tile2D::createGameObject();

    auto map = tileMap->attachComponent<TileMap>();
    map->load(
            "maps/map.bmp",
            Tile2D::resources().tileSets["tileset"],
            10,
            10
    );

    auto drawableMap = tileMap->attachComponent<DrawableMap>();
    drawableMap->setMap(map);
    drawableMap->setMapTexture(Tile2D::resources().tileSets["tileset"]->getMapTexture());

    // player

    auto player = Tile2D::createGameObject();
    player->tag = Tags::player;

    auto spaceshipBody = player->attachComponent<Body>();
    spaceshipBody->mass.set(100.0);
    spaceshipBody->position.set(Vec(4000.0, 8000.0));

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
    light->radius.set(400.0);

    auto playerController = player->attachComponent<PlayerController>();
    playerController->moveForce = 10000.0f;

    // camera

    camera = new Camera;
    camera->areaRect.set({0, 0, (double)Tile2D::window().w.get(), (double)Tile2D::window().h.get()});
    camera->position.bind(spaceshipBody->position);
    Tile2D::canvas().setCamera(camera);

    // dummy enemies

    spawnEnemy(spaceshipBody->position.get() + Vec(100.0, 0.0));
    spawnEnemy(spaceshipBody->position.get() + Vec(200.0, 0.0));
    spawnEnemy(spaceshipBody->position.get() + Vec(300.0, 0.0));
}

void SceneInGame::destroy() {
    delete camera;
}
