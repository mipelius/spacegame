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

#include "precompile.h"
#include "DebugBehaviour.h"
#include "Tile2D.h"
#include "Prefabs.h"
#include "EnemyAIBase.h"
#include "Input.h"
#include "LightSystem.h"
#include "TileMap.h"
#include "Window.h"

void DebugBehaviour::awake() {
    body_ = gameObject()->getComponent<Body>();
    sprite_ = gameObject()->getComponent<Sprite>();
}

void DebugBehaviour::update() {
    auto& keyboard = Tile2D::input().keyboard();

    if (keyboard.keyPressed(SDL_SCANCODE_1)) {
        auto walkingEnemy = Prefabs::walkingEnemy();
        walkingEnemy->transform().setPosition(transform()->getPosition());
        walkingEnemy->getComponent<EnemyAIBase>()->setTarget(transform());
    }
    if (keyboard.keyPressed(SDL_SCANCODE_2)) {
        auto crabKindOf = Prefabs::crabKindOf();
        crabKindOf->transform().setPosition(transform()->getPosition());
        crabKindOf->getComponent<EnemyAIBase>()->setTarget(transform());
    }
    if (keyboard.keyPressed(SDL_SCANCODE_3)) {
        auto fourwayCyclops = Prefabs::fourwayCyclops();
        fourwayCyclops->transform().setPosition(transform()->getPosition());
        fourwayCyclops->getComponent<EnemyAIBase>()->setTarget(transform());
    }
    if (keyboard.keyPressed(SDL_SCANCODE_4)) {
        auto twoHorn = Prefabs::twoHorn();
        twoHorn->transform().setPosition(transform()->getPosition());
        twoHorn->getComponent<EnemyAIBase>()->setTarget(transform());
    }
    if (keyboard.keyPressed(SDL_SCANCODE_5)) {
        auto boss = Prefabs::boss();
        boss->transform().setPosition(transform()->getPosition());
        boss->getComponent<EnemyAIBase>()->setTarget(transform());
    }
    if (keyboard.keyPressed(SDL_SCANCODE_RETURN)) {
        Tile2D::setIsDebugMode(!Tile2D::isDebugMode());
        if (Tile2D::isDebugMode()) {
            sprite_->setOpacity(0.5);
        } else {
            sprite_->setOpacity(1.0);
        }
    }
    if (keyboard.keyPressed(SDL_SCANCODE_P)) {
        std::cout << transform()->getPosition().x << "," << transform()->getPosition().y << "\n";
    }
    if (keyboard.keyPressed(SDL_SCANCODE_Q)) {
        transform()->setPosition({500.0, 250.0});
    }
    if (keyboard.keyPressed(SDL_SCANCODE_R)) {
        Tile2D::tileMap().load("data/maps/map.bmp", "data/maps/tileset.json");
    }
    if (keyboard.keyPressed(SDL_SCANCODE_A)) {
        Tile2D::lightSystem().setEnabled(!Tile2D::lightSystem().isEnabled());
    }
    if (keyboard.keyPressed(SDL_SCANCODE_S)) {
        Tile2D::lightSystem().setBlendedShadowsEnabled(!Tile2D::lightSystem().isBlendedShadowsEnabled());
    }
    if (keyboard.keyPressed(SDL_SCANCODE_D)) {
        Tile2D::lightSystem().setSoftShadowsEnabled(!Tile2D::lightSystem().isSoftShadowsEnabled());
    }
    if (keyboard.keyPressed(SDL_SCANCODE_V)) {
        Tile2D::window().setVsync(!Tile2D::window().getVsync());
    }
}

void DebugBehaviour::lateUpdate() {

}
