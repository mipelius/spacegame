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

#include "SceneInGame.h"
#include "BodyCollisionEventArgs.h"
#include "GameObject.h"
#include "Body.h"
#include "DrawableGroup.h"
#include "Text.h"
#include "Plot.h"
#include "Background.h"
#include "Sprite.h"
#include "CollisionShape.h"

#include "Tile2D.h"
#include "Camera.h"
#include "PlayerController.h"

void SceneInGame::init() {
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
    spaceshipBody->position.set(Vec(100.0, 100.0));
    player->addComponent(spaceshipBody);

    auto spaceshipSprite = Tile2D::create<Sprite>();
    spaceshipSprite->position.set({0, 0});
    spaceshipSprite->rect.set({-10, -10, 10, 10});
    spaceshipSprite->texturePtr.set(Tile2D::resources().textures["spaceship"]);
    spaceshipSprite->position.bind(spaceshipBody->position);
    spaceshipSprite->angle.bind(spaceshipBody->angle);
    player->addComponent(spaceshipSprite);

    auto playerController = Tile2D::create<PlayerController>();
    playerController->moveForce = 10000.0f;
    player->addComponent(playerController);

    // setup camera

    auto camera = Tile2D::create<Camera>();
    camera->areaRect.set({0, 0, (double)Tile2D::window().w.get(), (double)Tile2D::window().h.get()});
    camera->boundsRect.set({0, 0, 10000, 10000});
    camera->position.bind(spaceshipBody->position);
    Tile2D::canvas().setCamera(camera);

    //
}

void SceneInGame::destroy() {
    std::cout << "\"In game\" scene unloaded\n";
}
