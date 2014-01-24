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
#include "CollisionEventHandler.h"
#include "CollisionEventArgs.h"
#include "CollisionShape.h"
#include "Event.h"
#include "GameObject.h"
#include "GameWorld.h"
#include "Map.h"

static Texture* textureWalkingCreature = nullptr;

WalkingCreature::WalkingCreature(Point location, int maxHealth) : SpaceGameObject(Point(0,0), location, 0.0, nullptr, maxHealth) {
    this->collisionEvent->add(new CollisionEventHandler(onCollision));

    double size = 0.5;

    if (!textureWalkingCreature) textureWalkingCreature = new Texture("images/walking_creature.png");

    GameObject* obj = new GameObject(
            Point(0,0),
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

    this->collisionEvent->add(new CollisionEventHandler(onCollision));
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

void WalkingCreature::onCollision(GameEntity *entity, CollisionEventArgs *args) {
    WalkingCreature* walkingCreature = dynamic_cast<WalkingCreature*>(entity);
    if (walkingCreature) {
        walkingCreature->enableAbilityToMove();

        if (args->map) {
            if (entity->getVelocity().y != 0) {
                entity->setSpeed(Vector(entity->getSpeed().x, 0));
                Point newLocation = Point(args->newLocation.x, args->oldLocation.y);
                entity->setLocation(newLocation);
            }
            Point currentLocation = entity->getLocation();

            if (args->map->detectCollisionWith(entity)) {
                double factor = 0;
                if (entity->getSpeed().x > 0) factor = 1;
                if (entity->getSpeed().x < 0) factor = -1;
                entity->setLocation(
                        Point(
                                args->oldLocation.x + (factor * (args->map->getBlockW() * 1.5)),
                                args->oldLocation.y - (args->map->getBlockH() * 1.5)
                        )

                );
                if (args->map->detectCollisionWith(entity)) {
                    entity->setSpeed(Vector(0, 0));
                }
                else {
                    entity->setSpeed(Vector(-factor * 20, -150));
                }
                entity->setLocation(Point(args->oldLocation.x, args->newLocation.y));
                walkingCreature->setCollisionXAxis();

            }
        }
        else {

        }

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

            double angle = atan(deltaY / deltaX) * 180 / M_PI + 90;
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
