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

void PlayerController::awake() {
    body_ = gameObject()->getComponent<Body>();
    sprite_ = gameObject()->getComponent<Sprite>();

    shootTimer.setInterval(100);
    bombTimer.setInterval(100);
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

    if (keyboard.keyState(SDL_SCANCODE_LSHIFT)) {
        dropBomp_();
    }

    if (keyboard.keyState(SDL_SCANCODE_LCTRL)) {
        dropLight_();
    }

    if (keyboard.keyPressed(SDL_SCANCODE_ESCAPE)) {
        Tile2D::sceneManager().loadScene(Scenes::titleScreen);
    }
}

void PlayerController::shoot_() {
    if (!shootTimer.resetIfTimeIntervalPassed()) {
        return;
    }

    shootOnce_(Vecf(-10, -13).rotated(transform()->getRotation()));
    shootOnce_(Vecf(-10, 13).rotated(transform()->getRotation()));
}

void PlayerController::shootOnce_(Vecf offset) {
    auto laser = Prefabs::laser();
    laser->transform().setPosition(transform()->getPosition() + offset);
    laser->transform().setRotation(transform()->getRotation());

    auto laserBody = laser->getComponent<Body>();
    laserBody->setVelocity(Vecf::byAngle(transform()->getRotation(), 2000.0) + body_->getVelocity());

    auto laserCollider = laser->getComponent<PolygonCollider>();
    laserCollider->setLayer(ColliderLayers::playerAmmo);
}

void PlayerController::dropBomp_() {
    if (!bombTimer.resetIfTimeIntervalPassed()) {
        return;
    }

    auto bomb = Prefabs::bomb();
    bomb->transform() = *transform();
    bomb->getComponent<Body>()->setVelocity(body_->getVelocity() / 2 + Vecf(0, 100));

    auto bombCollider = bomb->getComponent<PolygonCollider>();
    bombCollider->setLayer(ColliderLayers::playerAmmo);
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
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    if (state[SDL_SCANCODE_SPACE]) {
        shoot_();
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

        float ambientLight = 1.0f - cameraPos.y / (Tile2D::tileMap().getActualH() / 2.0f);
        ambientLight /= 3.0f;
        Mathf::clamp(ambientLight, 0.0f, 0.1f);

        Tile2D::lightSystem().setAmbientLight(ambientLight);
    }
}