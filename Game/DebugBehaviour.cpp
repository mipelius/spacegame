// This file is part of SpaceGame.
// Copyright (C) 2018 Miika Pelkonen
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


#include <SDL2/SDL_events.h>
#include "DebugBehaviour.h"
#include "Spawner.h"
#include "Tile2D.h"
#include "Sprite.h"
#include "Body.h"

void DebugBehaviour::awake() {
    body_ = gameObject()->getComponent<Body>();
    sprite_ = gameObject()->getComponent<Sprite>();
}

void DebugBehaviour::update() {
    float size = 3.0f;
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_1 :
                    Spawner::spawnEnemy(
                            transform()->getPosition(),
                            "crab_kindof_silhouettish",
                            {{-10 * size, -25 * size}, {10 * size, -25 * size}, {10 * size, 25 * size}, {-10 * size, 25 * size}},
                            {-25 * size, -25 * size, 25 * size, 25 * size},
                            0.0f
                    );
                    break;
                case SDLK_2 :
                    Spawner::spawnEnemy(
                            transform()->getPosition(),
                            "crab_kindof_colored",
                            {{-10 * size, -25 * size}, {10 * size, -25 * size}, {10 * size, 25 * size}, {-10 * size, 25 * size}},
                            {-25 * size, -25 * size, 25 * size, 25 * size},
                            0.0f
                    );
                    break;
                case SDLK_3:
                    Spawner::spawnEnemy(
                            transform()->getPosition(),
                            "fourwaycyclops_silhouettish",
                            {{-30 * size, -24 * size}, {30 * size, -24 * size}, {30 * size, 24 * size}, {-30 * size, 24 * size}},
                            {-30 * size, -30 * size, 30 * size, 30 * size},
                            0.0f
                    );
                    break;
                case SDLK_4:
                    Spawner::spawnEnemy(
                            transform()->getPosition(),
                            "fourwaycyclops_colored",
                            {{-30 * size, -24 * size}, {30 * size, -24 * size}, {30 * size, 24 * size}, {-30 * size, 24 * size}},
                            {-30 * size, -30 * size, 30 * size, 30 * size},
                            0.0f
                    );
                    break;
                case SDLK_5:
                    Spawner::spawnEnemy(
                            transform()->getPosition(),
                            "twohorn_silhouettish",
                            {{-10 * size, -25 * size}, {10 * size, -25 * size}, {10 * size, 25 * size}, {-10 * size, 25 * size}},
                            {-25 * size, -25 * size, 25 * size, 25 * size},
                            0.0f
                    );
                    break;
                case SDLK_6:
                    Spawner::spawnEnemy(
                            transform()->getPosition(),
                            "twohorn_colored",
                            {{-10 * size, -25 * size}, {10 * size, -25 * size}, {10 * size, 25 * size}, {-10 * size, 25 * size}},
                            {-25 * size, -25 * size, 25 * size, 25 * size},
                            0.0f
                    );
                    break;
                case SDLK_RETURN:
                    Tile2D::isDebugMode = !Tile2D::isDebugMode;
                    if (Tile2D::isDebugMode) {
                        sprite_->setOpacity(0.5);
                    } else {
                        sprite_->setOpacity(1.0);
                    }
                    break;
                case SDLK_q:
                    transform()->setPosition({500.0, 250.0});
                    break;
                case SDLK_r:
                    Tile2D::tileMap().load("maps/map.bmp", "json/tileset.json");
                    break;
                case SDLK_s:
                    Tile2D::lightSystem().setBlendedShadowsEnabled(!Tile2D::lightSystem().isBlendedShadowsEnabled());
                    break;
                case SDLK_d:
                    Tile2D::lightSystem().setSoftShadowsEnabled(!Tile2D::lightSystem().isSoftShadowsEnabled());
                default:
                    break;
            }
        }
    }
}

void DebugBehaviour::lateUpdate() {

}
