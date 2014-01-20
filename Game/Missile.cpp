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

#include "Missile.h"
#include "../Core/GameObject.h"
#include "../Core/GameObjectGroup.h"
#include "Spaceship.h"

static Texture* textureTurret = nullptr;

Missile::Missile(Point location, double angle, double forceAmount, Vector initialSpeed) :
GameObjectGroup(
        Point(0, 0),
        location,
        angle,
        new CollisionShape(
                (Point[]){
                        Point(-2, -10),
                        Point(2, -10),
                        Point(2, 10),
                        Point(-2, 10)
                },
                4
        )
) {
    if (!textureTurret) textureTurret = new Texture("images/turret.png");

    GameObject *obj = new GameObject(
            Point(0, 0),
            Point(-5, -10),
            0.0,
            textureTurret,
            200,
            10,
            20,
            nullptr
    );

    this->add(obj);
    this->setAngle(angle);
    this->getCollisionEvent()->add(new CollisionEventHandler(onMissileCollision));
    this->setSpeed(initialSpeed);
    this->applyForce(Vector::byAngle(this->getAngle() - 90.0, forceAmount));
    this->timeAlive = 0;
}

void Missile::onMissileCollision(GameEntity *gameEntity, CollisionEventArgs *args) {
    if (args->map) {
        int x = (int)args->newLocation.x;
        int y = (int)args->newLocation.y;
        for (int i=-20; i<20; i+=5) {
            for (int j=-20; j<20; j+=5) {
                if (args->map->getValueActual(x + i, y + j)) {
                    args->map->setValueActual(x + i, y + j, 0);
                }
            }
        }
        for (int i=-60; i<60; i+=5) {
            for (int j=-60; j<60; j+=5) {
                if (args->map->getValueActual(x + i, y + j)) {
                    args->map->setValueActual(x + i, y + j, rand() % 2 + 1);
                }
            }
        }
    }

    SpaceGameObject* spaceGameObject = dynamic_cast<SpaceGameObject*>(args->otherEntity);
    if(spaceGameObject != 0) {
        spaceGameObject->damage(rand() % 5 + 5);
        spaceGameObject->applyForce(Vector::byAngle(gameEntity->getAngle() - 90, 6000));
    }

    gameEntity->die();
}

bool Missile::detectCollisionWith(GameEntity *entity) {
    if (entity == this->owner) return false;

    Missile* missile = dynamic_cast<Missile*>(entity);
    if(missile != 0) {
        return false;
    }

    return GameEntity::detectCollisionWith(entity);
}

void Missile::beforeStep(double timeElapsedSec) {
    GameEntity::beforeStep(timeElapsedSec);
    timeAlive += timeElapsedSec;
    if (timeAlive > 2) this->die();
}
