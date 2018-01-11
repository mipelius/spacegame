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

static void createPulseLight(Vec position) {
    auto obj = Tile2D::createGameObject();

    auto light = obj->attachComponent<PointLight>();
    light->radius.set(80.0);
    light->intensity.set(1.0);
    light->position.set(position);

//    auto sparkleSprite = obj->attachComponent<Sprite>();
//    sparkleSprite->rect.set({-20, -20, 20, 20});
//    sparkleSprite->color.set({1,1,1});
//    sparkleSprite->position.bind(light->position);
//    sparkleSprite->texturePtr.set(Tile2D::resources().textures["missile"]);

    auto pulseLightBehaviour = obj->attachComponent<PulseLightBehaviour>();
    pulseLightBehaviour->TTL = 1.0f;
}

static void createSparkle(Vec position, Vec velocity, Color color) {
    auto sparkle = Tile2D::createGameObject();

    auto sparkleBody = sparkle->attachComponent<Body>();
    sparkleBody->mass.set(10.0);
    sparkleBody->position.set(position);
    sparkleBody->velocity.set(velocity);
    sparkleBody->angle.set(velocity.angle());

    auto sparkleSprite = sparkle->attachComponent<Sprite>();
    sparkleSprite->rect.set({-2, -2, 2, 2});
    sparkleSprite->color.set(color);
    sparkleSprite->position.bind(sparkleBody->position);
    sparkleSprite->angle.bind(sparkleBody->angle);
    sparkleSprite->texturePtr.set(Tile2D::resources().textures["missile"]);

    auto sparkleBehaviour = sparkle->attachComponent<SparkleBehaviour>();
}

static void createSparkles(Vec position, Vec normal, Color color) {
    Vec& n = normal;
    Vec perp = Vec(n.y, -n.x);

    createSparkle(position, (n) * 1000, color);
    createSparkle(position, (perp + n) * 1000, color);
    createSparkle(position, ((perp * -1) + n) * 1000, color);
    createSparkle(position, (perp) * 1000, color);
    createSparkle(position, (perp) * -1000, color);
}
void MissileBehaviour::Body_MapCollisionEventHandler::handle(Body* body, MapCollisionEventArgs args) {
    body->gameObject()->destroy();
    Tile2D::physicsWorld().getMap()->setValueScaled(args.tileCoordinates, Tile2D::resources().tileSets["tileset"]->getEmptyBlock());

    createSparkles(args.tileCoordinates, args.contactNormal, {1, 1, 1});
    createPulseLight(body->position.get());
}

void MissileBehaviour::Body_BodyCollisionEventHandler::handle(Body* body, BodyCollisionEventArgs args) {
    if (args.otherBody->gameObject()->tag == Tags::enemy) {
        auto otherBody = args.otherBody->gameObject()->getComponent<Body>();
        otherBody->velocity.set(otherBody->velocity.get() + body->velocity.get() / 100.0);
        auto sprite = args.otherBody->gameObject()->getComponent<Sprite>();
        if (sprite->color.get().red > 0.9) {
            sprite->color.set({0, 1, 0});
        } else {
            sprite->color.set({1, 0, 0});
        }

        createSparkles(body->position.get(), args.contactNormal, {1, 0, 0});
        body->gameObject()->destroy();
    }
};

void MissileBehaviour::awake() {
    LimitedAgeBehaviour::awake();
    gameObject()->getComponent<Body>()->mapCollision.add(&body_mapCollisionEventHandler);
    gameObject()->getComponent<Body>()->bodyCollision.add(&body_bodyCollisionEventHandler);
    TTL = 1.0;
}

void MissileBehaviour::update() {
    LimitedAgeBehaviour::update();
}