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
#include "ColliderShape.h"

#include "Tile2D.h"
#include "Camera.h"
#include "PlayerController.h"

void SceneInGame::init() {

    Tile2D::physicsWorld().airDensity.set(0.0001);

    std::cout << "\"In game\" scene loaded\n";

    // background

    auto background = Tile2D::create<GameObject>();
    auto bg = Tile2D::create<Background>();
    bg->texturePtr.set(Tile2D::resources().textures["bg2"]);
    background->addComponent(bg);

    // player

    auto player = Tile2D::create<GameObject>();

    auto spaceshipBody = Tile2D::create<Body>();
    spaceshipBody->mass.set(100.0);
    spaceshipBody->position.set(Vec(0.0, 0.0));
    auto colliderShape = Tile2D::create<ColliderShape>();
    colliderShape->points.set({
          {20, 0},
          {-20, 20},
          {0, 0},
          {-20, -20}
    });
    spaceshipBody->setColliderShape(colliderShape);
    spaceshipBody->mapCollision.add(&body_mapCollisionEventHandler);
    player->addComponent(spaceshipBody);

    auto spaceshipSprite = Tile2D::create<Sprite>();
    spaceshipSprite->position.set({0, 0});
    spaceshipSprite->rect.set({-20, -20, 20, 20});
    spaceshipSprite->texturePtr.set(Tile2D::resources().textures["spaceship"]);
    spaceshipSprite->position.bind(spaceshipBody->position);
    spaceshipSprite->angle.bind(spaceshipBody->angle);
    player->addComponent(spaceshipSprite);

    auto playerController = Tile2D::create<PlayerController>();
    playerController->moveForce = 10000.0f;
    player->addComponent(playerController);

    // camera

    auto camera = Tile2D::create<Camera>();
    camera->areaRect.set({0, 0, (double)Tile2D::window().w.get(), (double)Tile2D::window().h.get()});
    camera->position.bind(spaceshipBody->position);
    Tile2D::canvas().setCamera(camera);

    // tile map

    auto tileMap = Tile2D::create<GameObject>();

    auto map = Tile2D::create<TileMap>();
    map->load(
            "maps/map.bmp",
            Tile2D::resources().tileSets["tileset"],
            10,
            10
    );
    tileMap->addComponent(map);

    auto drawableMap = Tile2D::create<DrawableMap>();
    drawableMap->setMap(map);
    drawableMap->setMapTexture(Tile2D::resources().tileSets["tileset"]->getMapTexture());
    tileMap->addComponent(drawableMap);
}

void SceneInGame::destroy() {
    std::cout << "\"In game\" scene unloaded\n";
}
