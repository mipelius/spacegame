//
// Created by Miika Pelkonen on 1/15/14.
// Copyright (c) 2014 ___MIIKAPELKONEN___. All rights reserved.
//

#include "Missile.h"
#include "GameObject.h"
#include "GameObjectGroup.h"
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

    Spaceship* spaceship = dynamic_cast<Spaceship*>(args->otherEntity);
    if(spaceship != 0) {
        spaceship->damage(rand() % 5 + 5);
        spaceship->applyForce(Vector::byAngle(gameEntity->getAngle() - 90, 600000));
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
