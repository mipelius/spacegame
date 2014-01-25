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
#include "Missile.h"
#include "Texture.h"
#include "Event.h"
#include "EntityCollisionEventArgs.h"
#include "EntityCollisionEventHandler.h"
#include "GameObject.h"
#include "Map.h"
#include "GameWorld.h"
#include "CollisionShape.h"

static Texture*textureMissile = nullptr;

Missile::Missile(Point location, double angle, double forceAmount, Vector initialSpeed)
  : SpaceGameObject
    (
        location,
        angle,
		nullptr,
        0
	) 
						
{
    if (!textureMissile) textureMissile = new Texture("images/missile.png");

    GameObject *obj = new GameObject(
            Point(-10, -5),
            0.0,
            textureMissile,
            200,
            20,
            10,
            nullptr
    );

    this->add(obj);

    Point missileShapePoints[] = {
            Point(-10, -2),
            Point(-10, 2),
            Point(10, 2),
            Point(10, -2)
    };

    this->setCollisionShape(new CollisionShape(missileShapePoints, 4));

    this->setAngle(angle);
    this->setSpeed(initialSpeed);
    this->applyForce(Vector::byAngle(this->getAngle(), forceAmount));
    this->timeAlive = 0;
}

void Missile::beforeEntityCollisionDetection(GameEntity* otherEntity) {
    GameEntity::beforeEntityCollisionDetection(otherEntity);

    SpaceGameObject* spaceGameObject = dynamic_cast<SpaceGameObject*>(otherEntity);
    if(spaceGameObject != 0) {
        if (spaceGameObject->getTeam() == this->getTeam()) {
            ignoreEntityCollisionDetection();
            return;
        }
    }

    if (otherEntity == this->owner) {
        ignoreEntityCollisionDetection();
        return;
    }

    Missile* missile = dynamic_cast<Missile*>(otherEntity);
    if(missile != 0) {
        ignoreEntityCollisionDetection();
    }
}

void Missile::beforeStep(double timeElapsedSec) {
    GameEntity::beforeStep(timeElapsedSec);
    timeAlive += timeElapsedSec;
    if (timeAlive > 2.0) this->die();
}

void Missile::onEntityCollision(GameEntity *otherEntity) {
    SpaceGameObject* spaceGameObject = dynamic_cast<SpaceGameObject*>(otherEntity);
    if(spaceGameObject != 0) {
        spaceGameObject->damage(rand() % 5 + 5);
        spaceGameObject->applyForce(Vector::byAngle(this->getAngle(), 6000));
    }

    this->die();
}

void Missile::onMapCollision() {
    int x = (int)location.x;
    int y = (int)location.y;

    for (int i=-20; i<20; i+=5) {
        for (int j=-20; j<20; j+=5) {
            if (gameWorld->getMap()->getValueActual(x + i, y + j)) {
                gameWorld->getMap()->setValueActual(x + i, y + j, 0);
            }
        }
    }

    for (int i=-60; i<60; i+=5) {
        for (int j=-60; j<60; j+=5) {
            if (gameWorld->getMap()->getValueActual(x + i, y + j)) {
                gameWorld->getMap()->setValueActual(x + i, y + j, (unsigned char)rand() % 2 + 1);
            }
        }
    }

    this->die();
}
