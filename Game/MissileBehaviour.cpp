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

void MissileBehaviour::Body_MapCollisionEventHandler::handle(Body* body, MapCollisionEventArgs args) {
    body->gameObject()->destroy();
    Tile2D::physicsWorld().getMap()->setValueScaled(args.tileCoordinates, nullptr);

    // funny sparkle effect

    Vec& n = args.contactNormal;
    Vec perp = Vec(n.y, -n.x);

    createSparkle(args.tileCoordinates, (n) * 1000);
    createSparkle(args.tileCoordinates, (perp + n) * 1000);
    createSparkle(args.tileCoordinates, ((perp * -1) + n) * 1000);
    createSparkle(args.tileCoordinates, (perp) * 1000);
    createSparkle(args.tileCoordinates, (perp) * -1000);

}

void MissileBehaviour::Body_MapCollisionEventHandler::createSparkle(Vec position, Vec velocity) {
    auto sparkle = Tile2D::create<GameObject>();

    auto sparkleBody = Tile2D::create<Body>();
    sparkleBody->mass.set(10.0);
    sparkleBody->position.set(position);
    sparkleBody->velocity.set(velocity);
    sparkleBody->angle.set(velocity.angle());
    sparkle->addComponent(sparkleBody);

    auto sparkleSprite = Tile2D::create<Sprite>();
    sparkleSprite->rect.set({-2, -2, 2, 2});
    sparkleSprite->color.set({1, 1, 1});
    sparkleSprite->position.bind(sparkleBody->position);
    sparkleSprite->angle.bind(sparkleBody->angle);
    sparkleSprite->texturePtr.set(Tile2D::resources().textures["missile"]);
    sparkle->addComponent(sparkleSprite);

    auto sparkleBehaviour = Tile2D::create<SparkleBehaviour>();
    sparkle->addComponent(sparkleBehaviour);
}

void MissileBehaviour::awake() {
    gameObject()->getComponent<Body>()->mapCollision.add(&body_mapCollisionEventHandler);
}

void MissileBehaviour::update() {

}