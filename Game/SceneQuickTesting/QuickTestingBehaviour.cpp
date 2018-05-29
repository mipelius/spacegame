// MIT License
//
// This file is part of SpaceGame.
// Copyright (c) 2014-2018 Miika Pelkonen
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.



#include "Sprite.h"
#include "Tile2D.h"
#include "Input.h"
#include "QuickTestingBehaviour.h"
#include "PathFinder.h"
#include "TileMap.h"
#include "TileSet.h"
#include "Camera.h"
#include "Window.h"
#include "GameObject.h"

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
