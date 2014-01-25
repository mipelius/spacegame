// This file is part of SpaceGame.
// Copyright (C) 2014 Miika Pelkonen
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
#include "WalkingCreature.h"
#include "Missile.h"
#include "Texture.h"
#include "EntityCollisionEventHandler.h"
#include "EntityCollisionEventArgs.h"
#include "CollisionShape.h"
#include "Event.h"
#include "GameObject.h"
#include "GameWorld.h"
#include "Map.h"

static Texture* textureWalkingCreature = nullptr;

WalkingCreature::WalkingCreature(Point location, int maxHealth) : SpaceGameObject(location, 0.0, nullptr, maxHealth) {

    double size = 0.5;

    if (!textureWalkingCreature) textureWalkingCreature = new Texture("images/walking_creature.png");

    GameObject* obj = new GameObject(
            Point(-20 * size, -30 * size),
            0.0,
            textureWalkingCreature,
            100,
            40 * size,
            60 * size,
            nullptr

    );

	this->add(obj);

	Point points[] = {
		Point(-20 * size, -30 * size),
		Point(20 * size, -30 * size),
		Point(20 * size, 30 * size),
		Point(-20 * size, 30 * size)
	};
	this->setCollisionShape(
		new CollisionShape(points, 4));

    isAbleToMove = false;
}

void WalkingCreature::walkLeft() {
    if (getSpeed().x > 0) {
        this->applyForce(Vector(-10000, 0));
        return;
    }

    if (isAbleToMove) {
        if (getSpeed().length() < 2000) this->applyForce(Vector(-500, 0));
    }
    else this->applyForce(Vector(-100, 0));
    _isWalking = true;
}

void WalkingCreature::walkRight() {
    if (getSpeed().x < 0) {
        this->applyForce(Vector(10000, 0));
        return;
    }

    if (isAbleToMove) {
        if (getSpeed().length() < 2000) this->applyForce(Vector(500, 0));
    }
    else this->applyForce(Vector(100, 0));
    _isWalking = true;
}

void WalkingCreature::jump() {

    if (isAbleToMove) {
        this->applyForce(Vector(0, -30000));
    }
    disableAbilityToMove();
}

void WalkingCreature::onMapCollision() {
    this->enableAbilityToMove();

    if (this->getVelocity().y != 0) {
        this->setSpeed(Vector(this->getSpeed().x, 0));
        Point newLocation = Point(location.x, this->getLocationBeforeUpdate().y);
        this->setLocation(newLocation);
    }

    Point currentLocation = this->getLocation();

    if (this->gameWorld->getMap()->detectCollisionWith(this)) {
        double factor = 0;
        if (this->getSpeed().x > 0) factor = 1;
        if (this->getSpeed().x < 0) factor = -1;

        this->setLocation(
                Point(
                        this->getLocationBeforeUpdate().x + (factor * (this->gameWorld->getMap()->getBlockW() * 1.5)),
                        this->getLocationBeforeUpdate().y - (this->gameWorld->getMap()->getBlockH() * 1.5)
                )
        );

        if (this->gameWorld->getMap()->detectCollisionWith(this)) {
            this->setSpeed(Vector(0, 0));
        }

        else {
            this->setSpeed(Vector(-factor * 20, -150));
        }
        this->setLocation(Point(getLocationBeforeUpdate().x, location.y));
        this->setCollisionXAxis();
    }
}

void WalkingCreature::disableAbilityToMove() {
    isAbleToMove = false;
}

void WalkingCreature::enableAbilityToMove() {
    isAbleToMove = true;
}

void WalkingCreature::setWalking() {
    _isWalking = true;
}

bool WalkingCreature::isWalking() {
    return _isWalking;
}

void WalkingCreature::setNotWalking() {
    _isWalking = false;
}

void WalkingCreature::beforeStep(double timeElapsedSeconds) {
    if (target) {
        if (target->getLocation().x > this->getLocation().x && rand() % 3) this->walkRight();
        else if (target->getLocation().x < this->getLocation().x && rand() % 3) this->walkLeft();

        if (!(rand() % 100)) {
            double deltaY = target->getLocation().y - this->getLocation().y;
            double deltaX = target->getLocation().x - this->getLocation().x;
            if (deltaX == 0) deltaX = 0.1;
            if (deltaY == 0) deltaY = 0.1;

            double angle = atan(deltaY / deltaX) * 180 / M_PI;
            if (deltaX < 0) angle -= 180;

			Point missileShapePoints[] = {
				Point(-2, -10),
				Point(2, -10),
				Point(2, 10),
				Point(-2, 10)
			};

			Missile* missile = new Missile(
				this->getLocation(),
				angle,
				2000000,
				Vector(0, 0),
				new CollisionShape(missileShapePoints, 4)
            );

            missile->setOwner(this);

            this->getWorld()->addEntity(missile);

        };
    }

//    if (!this->isWalking()) this->setSpeed(Vector(this->getSpeed().x * 0.5, this->getSpeed().y));
//    this->setNotWalking();

    if (collidedInLastStepXAxis) this->setSpeed(Vector(0, this->speed.y));
    collidedInLastStepXAxis = false;
}

void WalkingCreature::setCollisionXAxis() {
    collidedInLastStepXAxis = true;
}

void WalkingCreature::setTarget(GameEntity *target) {
    this->target = target;
}
