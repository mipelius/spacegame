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
#include "Health.h"

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
    PolygonCollider* collider = gameObject()->getComponent<PolygonCollider>();

    collider->terrainCollision.add([] (PolygonCollider* collider, TerrainCollisionEventArgs args) {
        collider->gameObject()->destroy();
        Tile2D::tileMap().setValueScaled(args.tileCoordinates, Tile2D::tileMap().getTileSet()->getEmptyBlock());

        createSparkles(args.tileCoordinates, args.contactNormal, {1, 1, 1});
        createPulseLight(collider->transform()->getPosition());
    });

    collider->collision.add([] (PolygonCollider* collider, CollisionEventArgs args) {
        if (args.otherCollider->gameObject()->tag == Tags::enemy) {
            auto otherBody = args.otherCollider->gameObject()->getComponent<Body>();
            float scaleX = args.otherCollider->transform()->getScale().x;
            args.otherCollider->transform()->setScale({-scaleX, 1});

            auto missileBody = collider->gameObject()->getComponent<Body>();

            otherBody->setVelocity(otherBody->getVelocity() + missileBody->getVelocity() / 100.0);
            createSparkles(collider->transform()->getPosition(), args.contactNormal, {1, 0, 0});
            missileBody->gameObject()->destroy();

            auto health = args.otherCollider->gameObject()->getComponent<Health>();

            if (health != nullptr) {
                health->damage(10, missileBody->gameObject());
            }
        }
    });
}

void MissileBehaviour::update() {
    LimitedAgeBehaviour::update();
}

void MissileBehaviour::lateUpdate() { }

