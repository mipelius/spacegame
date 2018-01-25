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

#include "Tile2D.h"
#include "PlayerController.h"
#include "MissileBehaviour.h"
#include "BombBehaviour.h"
#include "Tile2DMath.h"
#include "Spawner.h"

void PlayerController::awake() {
    body_ = gameObject()->getComponent<Body>();
    sprite_ = gameObject()->getComponent<Sprite>();
    lastShotTimestamp_ = SDL_GetTicks();
}

void PlayerController::update() {
    // read input -> actions
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    float angularVelocity = 0;

    if (state[SDL_SCANCODE_UP]) {
        Vecd force = Vecd::byAngle(transform()->getRotation(), moveForce);
        body_->applyForce(force);
    }
    if (state[SDL_SCANCODE_LEFT]) {
        angularVelocity -= 5;
    }
    if (state[SDL_SCANCODE_RIGHT]) {
        angularVelocity += 5;
    }

    body_->setAngularVelocity(angularVelocity);

    if (state[SDL_SCANCODE_SPACE]) {
        shoot_();
    }

    if (state[SDL_SCANCODE_LSHIFT]) {
        dropBomp_();
    }

    if (state[SDL_SCANCODE_LCTRL]) {
        dropLight_();
    }
}

void PlayerController::shoot_() {
    if (SDL_GetTicks() - lastShotTimestamp_ < shootingInterval_) {
        return;
    }
    lastShotTimestamp_ = SDL_GetTicks();

    shootOnce({0.0f, 0.0f});
    shootOnce(Vecf(-10, -15).rotated(transform()->getRotation()));
    shootOnce(Vecf(-10, 15).rotated(transform()->getRotation()));
}

void PlayerController::shootOnce(Vecf offset) {
    auto missile = Tile2D::createGameObject();
    missile->transform().setPosition(transform()->getPosition() + offset);
    missile->transform().setRotation(transform()->getRotation());

    auto missileBody = missile->attachComponent<Body>();
    missileBody->setMass(10.0);
    missileBody->setDrag(0.2);
    missileBody->setVelocity(Vecf::byAngle(transform()->getRotation(), 20000.0) + body_->getVelocity());

    auto collider = missile->attachComponent<PolygonCollider>();
    collider->setPoints({
                                {-18, -5},
                                {18,  -5},
                                {18,  5},
                                {-18, 5}
                        });

    auto missileSprite = missile->attachComponent<Sprite>();
    missileSprite->setRect({-20,-5,20,5});
    missileSprite->setTexturePtr(Tile2D::resources().textures["missile"]);

    auto missileBehaviour = missile->attachComponent<MissileBehaviour>();

    auto missileLight = missile->attachComponent<PointLight>();
    missileLight->setRadius(80.0);
    missileLight->setIntensity(1.0);
}

void PlayerController::lateUpdate() {
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

        if (cameraPos.x < cameraBounds.x1) {
            cameraPos.x = cameraBounds.x1;
        }
        if (cameraPos.y < cameraBounds.y1) {
            cameraPos.y = cameraBounds.y1;
        }
        if (cameraPos.x > cameraBounds.x2) {
            cameraPos.x = cameraBounds.x2;
        }
        if (cameraPos.y > cameraBounds.y2) {
            cameraPos.y = cameraBounds.y2;
        }

        camera->setPosition(cameraPos);

        float ambientLight = 1.0f - cameraPos.y / (Tile2D::tileMap().getActualH() / 2.0f);
        ambientLight /= 3.0f;
        Mathf::clamp(ambientLight, 0.0f, 0.1f);

        Tile2D::lightSystem().setAmbientLight(ambientLight);
    }
}

void PlayerController::dropBomp_() {
    if (SDL_GetTicks() - lastBombTimestamp_ < bombingInterval_) {
        return;
    }
    lastBombTimestamp_ = SDL_GetTicks();

    auto bomp = Tile2D::createGameObject();
    bomp->transform().setPosition(transform()->getPosition());
    bomp->transform().setRotation(transform()->getRotation());

    auto bompBody = bomp->attachComponent<Body>();
    bompBody->setMass(50);
    bompBody->setVelocity(body_->getVelocity() / 2 + Vecf(0, 1000));

    auto bompCollider = bomp->attachComponent<PolygonCollider>();
    bompCollider->setPoints({{-9, -4}, {9, -4}, {9, 4}, {-9, 4}});

    auto bompSprite = bomp->attachComponent<Sprite>();
    bompSprite->setTexturePtr(Tile2D::resources().textures["bomb"]);
    bompSprite->setRect({-10, -10, 10, 10});

    auto bombBehaviour = bomp->attachComponent<BombBehaviour>();
}

void PlayerController::dropLight_() {
    if (SDL_GetTicks() - lastLightDropTimestamp_ < lightDropInterval_) {
        return;
    }
    lastLightDropTimestamp_ = SDL_GetTicks();

    auto light = Tile2D::createGameObject();
    light->transform().setPosition(transform()->getPosition());
    light->transform().setRotation(0.0f);

    auto lightBody = light->attachComponent<Body>();
    lightBody->setMass(10.0);
    lightBody->setVelocity(Vecf(0, 0));

    auto collider = light->attachComponent<PolygonCollider>();
    collider->setPoints({
            {-6, -6},
            {6,  -6},
            {6,  6},
            {-6, 6}
    });

    auto lightSprite = light->attachComponent<Sprite>();
    lightSprite->setRect({-40, -40, 40, 40});
    lightSprite->setTexturePtr(Tile2D::resources().textures["light"]);

    auto lightLight = light->attachComponent<PointLight>();
    lightLight->setRadius(100.0);
    lightLight->setIntensity(1.0);

}
