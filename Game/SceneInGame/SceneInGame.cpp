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

void createBackground(Rect area, const char* texture, Color color) {
    auto background = Tile2D::createGameObject();
    auto bg = background->attachComponent<Background>();
    bg->setRatio(0.5f);
    bg->setTexturePtr(Tile2D::resources().textures[texture]);
    bg->setColor(color);
    bg->setOpacity(0.0f);
    auto bgBehaviour = background->attachComponent<BackgroundBehaviour>();
    bgBehaviour->setArea(area);
}

void SceneInGame::init() {
    // Scene setup: tile map, physics, light system
    Tile2D::tileMap().load("maps/map.bmp", "json/tileset.json");
    Tile2D::lightSystem().setAmbientLight(0.5f);
    Tile2D::physicsWorld().setAirDensity(0.0001);

    // backgrounds
    createBackground({0, 0, 3300, 2400},       "bg2", {0.8, 1.0, 0.5}); // upper left corner
    createBackground({3300, 0, 6400, 2400},    "bg2", {0.2, 0.8, 1.0}); // upper right corner
    createBackground({0, 2400, 3300, 4800},    "bg1", {0.6, 0.3, 0.3}); // bottom left corner
    createBackground({3300, 2400, 6400, 4800}, "bg1", {0.3, 0.3, 0.6}); // bottom right corner

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

    spaceshipSprite->setRect({-30, -30, 30, 30});
    spaceshipSprite->setTexturePtr(Tile2D::resources().textures["spaceship_small"]);

    // uncomment to use old spaceship sprite and use this later!
//    spaceshipSprite->setRect({-20, -20, 20, 20});
//    spaceshipSprite->setTexturePtr(Tile2D::resources().textures["spaceship"]);

    auto light = player->attachComponent<PointLight>();
    light->setIntensity(1.0);
    light->setRadius(500.0);

    auto playerController = player->attachComponent<PlayerController>();
    playerController->moveForce = 10000.0f;

    // camera
    camera = new Camera;
    camera->setAreaRect({0, 0, (float)Tile2D::window().getW(), (float)Tile2D::window().getH()});
    Tile2D::canvas().setCamera(camera);
}

void SceneInGame::destroy() {
    delete camera;
}
