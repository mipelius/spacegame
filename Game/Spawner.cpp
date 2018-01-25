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


#include "Spawner.h"
#include "Tile2D.h"
#include "Tags.h"
#include "Sprite.h"
#include "PolygonCollider.h"

GameObject* Spawner::spawnEnemy(
        Vecf pos,
        std::string textureName,
        std::vector<Vecf> colliderPoints,
        Rect spriteRect,
        float gravityFactor
) {
    auto enemy = Tile2D::createGameObject();
    enemy->transform().setPosition(pos);
    enemy->transform().setRotation(0.0f);

    enemy->tag = Tags::enemy;

    auto enemyBody = enemy->attachComponent<Body>();
    enemyBody->setMass(100.0);
    enemyBody->setGravityFactor(gravityFactor);

    auto polygonCollider = enemy->attachComponent<PolygonCollider>();
    polygonCollider->setPoints(colliderPoints);

    auto enemySprite = enemy->attachComponent<Sprite>();
    enemySprite->setRect(spriteRect);
    enemySprite->setTexturePtr(Tile2D::resources().textures[textureName]);
    enemySprite->setColor({1, 1, 1});

    return enemy;
}