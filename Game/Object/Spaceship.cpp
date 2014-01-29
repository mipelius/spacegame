// This file is part of SpaceGame.
// Copyright (C) 2014  Miika Pelkonen
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

#include "precompile.h"
#include "Spaceship.h"
#include "Missile.h"
#include "Texture.h"
#include "GameObject.h"
#include "CollisionShape.h"
#include "GameWorld.h"
#include "AnimatedTexture.h"

static Texture* textureSpaceShip = nullptr;
static Texture* textureRocketFire = nullptr;

Spaceship::Spaceship(Point location, int maxHealth, int size) :
SpaceGameObject(location, 0.0, nullptr, maxHealth) {
    if (!textureSpaceShip) textureSpaceShip = new Texture("images/spaceship.png");
    if (!textureRocketFire) textureRocketFire = new Texture("images/anim_rocket_fire.png");

    this->rocketFire = new AnimatedTexture(8, 20, textureRocketFire);

    GameObject *objectSpaceShip = new GameObject(
            Point(-16 * size, -16 * size),
            0.0,
            textureSpaceShip,
            1000,
            32 * size,
            32 * size,
            nullptr
    );

    GameObject *leftRocketFire = new GameObject(
            Point((-16 - 16)* size, -17 * size),
            0.0,
            rocketFire,
            0,
            16 * size,
            8 * size
    );

    GameObject *rightRocketFire = new GameObject(
            Point((-16 - 16)* size, (17 - 8) * size),
            0.0,
            rocketFire,
            0,
            16 * size,
            8 * size
    );

    Point collisionPoints[] = {
            Point(-16 * size, -16 * size),
            Point(16 * size, 0 * size),
            Point(-16 * size, 16 * size),
    };

    this->setCollisionShape(new CollisionShape(collisionPoints, 3));
    this->add(objectSpaceShip);
    this->add(leftRocketFire);
    this->add(rightRocketFire);

    this->health = maxHealth;
    this->lastTimeShot = 0;
    this->shootingDelay = 0;
    this->_isStuck = false;
}

void Spaceship::shoot() {
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastTimeShot > shootingDelay) {
        forceShoot();
        lastTimeShot = currentTime;
    }
}

void Spaceship::shootOnce(Point startPoint) {
    Missile* missile = new Missile(this->location + Vector(startPoint.x, startPoint.y), this->angle, 2000000, this->speed);
    missile->setTeam(this->getTeam());
    missile->setOwner(this);
    gameWorld->addEntity(missile);
}

void Spaceship::setShootingSpeed(Uint32 shootingPerSecond) {
    if (shootingPerSecond == 0) shootingDelay = 9999;
    else shootingDelay = 1000 / shootingPerSecond;
}

void Spaceship::forceShoot() {
    Vector vector = Vector::byAngle(angle, -22);
    shootOnce(Point(0, 0) + (Vector::byAngle(angle + 45, 17) + vector));
    shootOnce(Point(0, 0) + (Vector::byAngle(angle - 45, 17) + vector));
    shootOnce(Point(0, 0) + (Vector::byAngle(angle, 10)));
}

void Spaceship::setStuck() {
    _isStuck = true;
}

bool Spaceship::isStuck() {
    return _isStuck;
}

void Spaceship::setNotStuck() {
    _isStuck = false;
}

void Spaceship::onEntityCollision(GameEntity *otherEntity) {
    GameEntity::onEntityCollision(otherEntity);
    this->speed = Vector(0, 0);
    this->location = getLocationBeforeUpdate();
}

void Spaceship::onMapCollision() {
    GameEntity::onMapCollision();
    this->speed = Vector(0, 0);
    this->location = getLocationBeforeUpdate();
}

void Spaceship::accelerate() {
    _isAccelerating = true;
}

void Spaceship::beforeStep(double timeElapsedSec) {
    SpaceGameObject::beforeStep(timeElapsedSec);
    if (_isAccelerating) {
        _isAccelerating = false;
        rocketFire->play();
    }
    else {
        rocketFire->stop();
    }

}
