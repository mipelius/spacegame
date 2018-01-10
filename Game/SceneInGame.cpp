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

void SceneInGame::init() {
    Tile2D::physicsWorld().airDensity.set(0.0001);

    // background

    auto background = Tile2D::createGameObject();
    auto bg = background->attachComponent<Background>();
    bg->ratio.set(0.5f);
    bg->texturePtr.set(Tile2D::resources().textures["bg2"]);

    // player

    auto player = Tile2D::createGameObject();

    auto spaceshipBody = player->attachComponent<Body>();
    spaceshipBody->mass.set(100.0);
    spaceshipBody->position.set(Vec(4000.0, 8000.0));

    auto polygonCollider = player->attachComponent<PolygonCollider>(); // TODO: PolygonCollider : Tile2DComponent
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

    auto playerController = player->attachComponent<PlayerController>();
    playerController->moveForce = 10000.0f;

    // camera

    auto camera = new Camera(); // TODO: don't use new! --> maybe only one camera?
    camera->areaRect.set({0, 0, (double)Tile2D::window().w.get(), (double)Tile2D::window().h.get()});
    camera->position.bind(spaceshipBody->position);
    Tile2D::canvas().setCamera(camera);

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
}

void SceneInGame::destroy() { }
