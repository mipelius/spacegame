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

void DebugBehaviour::awake() {
    body_ = gameObject()->getComponent<Body>();
    sprite_ = gameObject()->getComponent<Sprite>();
}

void DebugBehaviour::update() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN) {
            if (spawnEnemy_) {
                switch (event.key.keysym.sym) {
                    case SDLK_1 :
                        Spawner::spawnEnemy(
                                transform()->getPosition(),
                                "walking_alien_green",
                                {{-10, -25}, {10, -25}, {10, 25}, {-10, 25}},
                                {-25, -25, 25, 25},
                                1.0f
                        );
                        break;
                    case SDLK_2 :
                        Spawner::spawnEnemy(
                                transform()->getPosition(),
                                "walking_alien_blue",
                                {{-10, -25}, {10, -25}, {10, 25}, {-10, 25}},
                                {-25, -25, 25, 25},
                                1.0f
                        );
                        break;

                    case SDLK_3:
                        Spawner::spawnEnemy(
                                transform()->getPosition(),
                                "yoda_kindof_red",
                                {{-30, -24}, {30, -24}, {30, 24}, {-30, 24}},
                                {-30, -30, 30, 30},
                                1.0f
                        );
                        break;
                    case SDLK_4:
                        Spawner::spawnEnemy(
                                transform()->getPosition(),
                                "yoda_kindof_blue",
                                {{-30, -24}, {30, -24}, {30, 24}, {-30, 24}},
                                {-30, -30, 30, 30},
                                1.0f
                        );
                        break;
                    case SDLK_5:
                        Spawner::spawnEnemy(
                                transform()->getPosition(),
                                "flying_alien_redwine",
                                {{-10, -25}, {10, -25}, {10, 25}, {-10, 25}},
                                {-25, -25, 25, 25},
                                0.0f
                        );
                        break;
                    case SDLK_6:
                        Spawner::spawnEnemy(
                                transform()->getPosition(),
                                "flying_alien_blue",
                                {{-10, -25}, {10, -25}, {10, 25}, {-10, 25}},
                                {-25, -25, 25, 25},
                                0.0f
                        );
                        break;
                    case SDLK_v:
                        spawnEnemy_ = false;
                        break;
                    default:
                        break;
                }

                continue;
            }

            if (event.key.keysym.sym == SDLK_RETURN) {
                Tile2D::isDebugMode = !Tile2D::isDebugMode;
                if (Tile2D::isDebugMode) {
                    sprite_->setOpacity(0.5);
                } else {
                    sprite_->setOpacity(1.0);
                }
            }
            if (event.key.keysym.sym == SDLK_q) {
                transform()->setPosition({500.0, 250.0});
            }
            if (event.key.keysym.sym == SDLK_r) {
                // reload map
                Tile2D::tileMap().load("maps/map.bmp", "json/tileset.json");
            }
            if (event.key.keysym.sym == SDLK_s) {
                Tile2D::lightSystem().setBlendedShadowsEnabled(!Tile2D::lightSystem().isBlendedShadowsEnabled());
            }
            if (event.key.keysym.sym == SDLK_d) {
                Tile2D::lightSystem().setSoftShadowsEnabled(!Tile2D::lightSystem().isSoftShadowsEnabled());
            }
            if (event.key.keysym.sym == SDLK_v) {
                spawnEnemy_ = true;
            }
        }
    }
}

void DebugBehaviour::lateUpdate() {

}
