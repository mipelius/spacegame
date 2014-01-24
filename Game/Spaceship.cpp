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

static Texture* textureSpaceShip = nullptr;

Spaceship::Spaceship(Point location, int maxHealt, int size) :
SpaceGameObject(Point(0, 0), location, 0.0, nullptr, maxHealt) {
    if (!textureSpaceShip) textureSpaceShip = new Texture("images/spaceship.png");

    GameObject *objectSpaceShip = new GameObject(
            Point(0, 0),
            Point(-16 * size, -16 * size),
            0.0,
            textureSpaceShip,
            1000,
            32 * size,
            32 * size,
            nullptr
    );

    Point collisionPoints[] = {
            Point(0 * size, -16 * size),
            Point(16 * size, 16 * size),
            Point(-16 * size, 16 * size),
    };

    this->setCollisionShape(new CollisionShape(collisionPoints, 3));
    this->add(objectSpaceShip);

    this->health = maxHealt;
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
	Point missileShapePoints[] = {
		Point(-2, -10),
		Point(2, -10),
		Point(2, 10),
		Point(-2, 10)
	};

    Missile* missile = new Missile(this->location + Vector(startPoint.x, startPoint.y), this->angle, 2000000, this->speed,
		new CollisionShape(missileShapePoints, 4));
    missile->setOwner(this);
    gameWorld->addEntity(missile);
}

void Spaceship::setShootingSpeed(Uint32 shootingPerSecond) {
    if (shootingPerSecond == 0) shootingDelay = 9999;
    else shootingDelay = 1000 / shootingPerSecond;
}

void Spaceship::forceShoot() {
    Vector vector = Vector::byAngle(angle - 90, -22);
    shootOnce(Point(0, 0) + (Vector::byAngle(angle - 90 + 45, 17) + vector));
    shootOnce(Point(0, 0) + (Vector::byAngle(angle - 90 - 45, 17) + vector));
    shootOnce(Point(0, 0) + (Vector::byAngle(angle - 90, 10)));
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
