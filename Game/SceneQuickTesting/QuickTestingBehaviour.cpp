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


#include "Sprite.h"
#include "Tile2D.h"
#include "Input.h"
#include "QuickTestingBehaviour.h"
#include "PathFinder.h"
#include "TileMap.h"
#include "TileSet.h"
#include "Camera.h"
#include "Window.h"

void QuickTestingBehaviour::awake() {
    auto createRect = [] (Color color) {
        auto endPoint = Tile2D::createGameObject();
        Vecf position = Vecf(8.0f, 8.0f) * 400.0f + Vecf(4.0f, 4.0f);
        endPoint->transform().setPosition(position);
        auto spriteEndPoint_ = endPoint->attachComponent<Sprite>();
        spriteEndPoint_->setRect({-4.0f, -4.0f, 4.0f, 4.0f});
        spriteEndPoint_->setColor(color);
        return endPoint;
    };

    auto camera = Tile2D::canvas().getCamera();
    camera->setAreaRect({0, 0, (float)Tile2D::window().getW() * 2, (float)Tile2D::window().getH() * 2});

    Tile2D::canvas().getCamera()->setPosition(Vecf(8.0f, 8.0f) * 400.0f);

    lineStart_ = createRect({1.0f, 0.0f, 0.0f});
    lineGoal_ = createRect({0.0f, 1.0f, 0.0f});
    collisionPoint_ = createRect({1.0f, 1.0f, 1.0f});
}

void QuickTestingBehaviour::update() {
    auto& keyboard = Tile2D::input().keyboard();
    if (keyboard.keyPressed(SDL_SCANCODE_ESCAPE)) {
        Tile2D::quit();
    }

    auto& lineStartPosition = lineStart_->transform().getPosition();
    auto& lineGoalPosition  = lineGoal_->transform().getPosition();

    auto stepHorizontal = (float)Tile2D::tileMap().getTileSet()->getTileW();
    auto stepVertical = (float)Tile2D::tileMap().getTileSet()->getTileH();

    Vecf up     = {0.0f,            -stepVertical};
    Vecf down   = {0.0f,            stepVertical};
    Vecf left   = {-stepHorizontal, 0.0f};
    Vecf right  = {stepHorizontal,  0.0f};

    if (keyboard.keyState(SDL_SCANCODE_W)) {
        lineStart_->transform().setPosition(lineStartPosition + up);
    }
    if (keyboard.keyState(SDL_SCANCODE_S)) {
        lineStart_->transform().setPosition(lineStartPosition + down);
    }
    if (keyboard.keyState(SDL_SCANCODE_A)) {
        lineStart_->transform().setPosition(lineStartPosition + left);
    }
    if (keyboard.keyState(SDL_SCANCODE_D)) {
        lineStart_->transform().setPosition(lineStartPosition + right);
    }
    if (keyboard.keyState(SDL_SCANCODE_UP)) {
        lineGoal_->transform().setPosition(lineGoalPosition + up);
    }
    if (keyboard.keyState(SDL_SCANCODE_DOWN)) {
        lineGoal_->transform().setPosition(lineGoalPosition + down);
    }
    if (keyboard.keyState(SDL_SCANCODE_LEFT)) {
        lineGoal_->transform().setPosition(lineGoalPosition + left);
    }
    if (keyboard.keyState(SDL_SCANCODE_RIGHT)) {
        lineGoal_->transform().setPosition(lineGoalPosition + right);
    }

    Vecf result;

    if (Tile2D::tileMap().castLine(lineStartPosition, lineGoalPosition, result)) {
        collisionPoint_->transform().setPosition(result);
        collisionPoint_->setIsActive(true);
    } else {
        collisionPoint_->setIsActive(false);
    }
}

void QuickTestingBehaviour::lateUpdate() {

}
