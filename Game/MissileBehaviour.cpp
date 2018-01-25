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

#include "Tile2D.h"
#include "MissileBehaviour.h"
#include "SparkleBehaviour.h"
#include "Tags.h"
#include "PulseLightBehaviour.h"

static void createPulseLight(Vecf position) {
    auto obj = Tile2D::createGameObject();
    obj->transform().setPosition(position);

    auto light = obj->attachComponent<PointLight>();
    light->setRadius(80.0);
    light->setIntensity(1.0);

    auto pulseLightBehaviour = obj->attachComponent<PulseLightBehaviour>();
    pulseLightBehaviour->TTL = 1.0f;
}

static void createSparkle(Vecf position, Vecf velocity, Color color) {
    auto sparkle = Tile2D::createGameObject();
    sparkle->transform().setPosition(position);
    sparkle->transform().setRotation(velocity.angle());


    auto sparkleBody = sparkle->attachComponent<Body>();
    sparkleBody->setMass(10.0);
    sparkleBody->setVelocity(velocity);

    auto sparkleSprite = sparkle->attachComponent<Sprite>();
    sparkleSprite->setRect({-20, -20, 20, 20});
    sparkleSprite->setColor(color);
    sparkleSprite->setTexturePtr(Tile2D::resources().textures["light"]);

    auto sparkleBehaviour = sparkle->attachComponent<SparkleBehaviour>();
}

static void createSparkles(Vecf position, Vecf normal, Color color) {
    Vecf& n = normal;
    Vecf perp = Vecf(n.y, -n.x);

    float div = 100.0;

    Vecf random = Vecf(rand() % 100, rand() % 100) / div;
    createSparkle(position, (n + random) * 1000, color);
    random = Vecf(rand() % 100, rand() % 100) / div;
    createSparkle(position, (perp + n + random) * 1000, color);
    random = Vecf(rand() % 100, rand() % 100) / div;
    createSparkle(position, ((perp * -1 + random) + n) * 1000, color);
    random = Vecf(rand() % 100, rand() % 100) / div;
    createSparkle(position, (perp + random) * 1000, color);
    random = Vecf(rand() % 100, rand() % 100) / div;
    createSparkle(position, (perp + random) * -1000, color);
}

void MissileBehaviour::awake() {
    LimitedAgeBehaviour::awake();
    TTL = 1.0;

    // collision event handlers

    gameObject()->getComponent<Body>()->mapCollision.add([] (Body* body, MapCollisionEventArgs args) {
        body->gameObject()->destroy();
        Tile2D::tileMap().setValueScaled(args.tileCoordinates, Tile2D::tileMap().getTileSet()->getEmptyBlock());

        createSparkles(args.tileCoordinates, args.contactNormal, {1, 1, 1});
        createPulseLight(body->transform()->getPosition());
    });

    gameObject()->getComponent<Body>()->bodyCollision.add([] (Body* body, BodyCollisionEventArgs args) {
        if (args.otherBody->gameObject()->tag == Tags::enemy) {
            auto otherBody = args.otherBody->gameObject()->getComponent<Body>();
            float scaleX = args.otherBody->transform()->getScale().x;
            args.otherBody->transform()->setScale({-scaleX, 1});
            otherBody->setVelocity(otherBody->getVelocity() + body->getVelocity() / 100.0);
            createSparkles(body->transform()->getPosition(), args.contactNormal, {1, 0, 0});
            body->gameObject()->destroy();
        }
    });
}

void MissileBehaviour::update() {
    LimitedAgeBehaviour::update();
}

void MissileBehaviour::lateUpdate() { }

