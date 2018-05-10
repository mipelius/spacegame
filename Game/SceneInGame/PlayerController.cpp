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


#include "Tile2D.h"
#include "PlayerController.h"
#include "Tile2DMath.h"
#include "Prefabs.h"
#include "ColliderLayers.h"
#include "Scenes.h"
#include "Input.h"
#include "SceneManager.h"
#include "Tilemap.h"
#include "LightSystem.h"
#include "Camera.h"
#include "Power.h"

void PlayerController::awake() {
    body_ = gameObject()->getComponent<Body>();
    sprite_ = gameObject()->getComponent<Sprite>();
    weaponSystem_ = gameObject()->getComponent<Inventory>();

    lightTimer.setInterval(500);
}

void PlayerController::update() {
    auto& keyboard = Tile2D::input().keyboard();

    float angularVelocity = 0;

    if (keyboard.keyState(SDL_SCANCODE_UP)) {
        Vecf force = Vecf::byAngle(transform()->getRotation(), moveForce);
        body_->applyForce(force);
    }
    if (keyboard.keyState(SDL_SCANCODE_LEFT)) {
        angularVelocity -= 300;
    }
    if (keyboard.keyState(SDL_SCANCODE_RIGHT)) {
        angularVelocity += 300;
    }

    body_->setAngularVelocity(angularVelocity);

    if (keyboard.keyState(SDL_SCANCODE_LCTRL)) {
        dropLight_();
    }

    if (keyboard.keyPressed(SDL_SCANCODE_ESCAPE)) {
        Tile2D::sceneManager().loadScene(Scenes::titleScreen);
    }

    // weapon slot selection

    if (keyboard.keyPressed(SDL_SCANCODE_1)) {
        weaponSystem_->selectItem(0);
    }
    if (keyboard.keyPressed(SDL_SCANCODE_2)) {
        weaponSystem_->selectItem(1);
    }
    if (keyboard.keyPressed(SDL_SCANCODE_3)) {
        weaponSystem_->selectItem(2);
    }
    if (keyboard.keyPressed(SDL_SCANCODE_4)) {
        weaponSystem_->selectItem(3);
    }
    if (keyboard.keyPressed(SDL_SCANCODE_5)) {
        weaponSystem_->selectItem(4);
    }
    if (keyboard.keyPressed(SDL_SCANCODE_6)) {
        weaponSystem_->selectItem(5);
    }
}

void PlayerController::dropLight_() {
    if (!lightTimer.resetIfTimeIntervalPassed()) {
        return;
    }

    auto light = Prefabs::light();
    light->transform().setPosition(transform()->getPosition());
    light->transform().setRotation(0.0f);
}

void PlayerController::lateUpdate() {
    if (Tile2D::input().keyboard().keyState(SDL_SCANCODE_SPACE)) {
        weaponSystem_->useSelectedItem();
    }

    // prevent player from going outside the world
    Vecf pos = transform()->getPosition();
    Vecf vel = body_->getVelocity();

    if (pos.x < 0) {
        pos.x = 0;
        vel.x = 0;
    }
    if (pos.x > Tile2D::tileMap().getActualW()) {
        pos.x = Tile2D::tileMap().getActualW();
        vel.x = 0;
    }
    if (pos.y < 0) {
        pos.y = 0;
        vel.y = 0;
    }
    if (pos.y > Tile2D::tileMap().getActualH()) {
        pos.y = Tile2D::tileMap().getActualH();
        vel.y = 0;
    }

    transform()->setPosition(pos);
    body_->setVelocity(vel);

    // set camera

    auto camera = Tile2D::canvas().getCamera();

    if (camera != nullptr && Tile2D::tileMap().isLoaded()) {
        Rect cameraBounds = {
                camera->getAreaRect().getWidth() / 2,
                camera->getAreaRect().getHeight() / 2,
                Tile2D::tileMap().getActualW() - camera->getAreaRect().getWidth() / 2,
                Tile2D::tileMap().getActualH() - camera->getAreaRect().getHeight() / 2
        };

        Vecf cameraPos = transform()->getPosition();

        Mathf::clamp(cameraPos, cameraBounds);

        camera->setPosition(cameraPos);
    }
}