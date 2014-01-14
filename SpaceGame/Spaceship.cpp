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

#include "Spaceship.h"

static Texture* textureSpaceShip = nullptr;
static Texture* textureTurret = nullptr;

Spaceship::Spaceship(Point location, int maxHealt) :
GameObjectGroup(Point(0, 0), location, nullptr) {
    if (!textureSpaceShip) textureSpaceShip = new Texture("images/spaceship.png");

    GameObject *objectSpaceShip = new GameObject(
            Point(0, 0),
            Point(-16, -16),
            textureSpaceShip,
            1000,
            32,
            32,
            nullptr
    );

    Point collisionPoints[] = {
            Point(0, -16),
            Point(16, 16),
            Point(-16, 16),
    };

    this->setCollisionShape(new CollisionShape(collisionPoints, 3));
    this->add(objectSpaceShip);

    this->health = maxHealt;
}


void Spaceship::addTurrets() {
    GameObject* leftTurret = createTurret(Point(4, 0), Point(-14, -5), 8, 16);
    GameObject* rightTurret = createTurret(Point(4, 0), Point(14, -5), 8, 16);
    GameObject* middleTurret = createTurret(Point(2.5, 5), Point(0, -15), 5, 10);

    this->add(leftTurret);
    this->add(rightTurret);
    this->add(middleTurret);
}

GameObject* Spaceship::createTurret(Point focus, Point location, int w, int h) {
    if (!textureTurret) textureTurret = new Texture("images/turret.png");

    GameObject *turret = new GameObject(
            focus,
            location,
            textureTurret,
            100,
            w,
            h,
            nullptr
    );

    return turret;
}

void Spaceship::shoot() {
    Vector vector = Vector::byAngle(angle - 90, -22);
    shootOnce(Point(0, 0) + Vector::byAngle(angle - 90 + 45, 17) + vector);
    shootOnce(Point(0, 0) + Vector::byAngle(angle - 90 - 45, 17) + vector);
    shootOnce(Point(0, 0) + Vector::byAngle(angle - 90, 10));
}

void Spaceship::onMissileCollision(GameEntity *gameEntity, CollisionEventArgs *args) {
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

    if (args->otherEntity && args->otherEntity == gameEntity->getOwner()) return;

    Spaceship* spaceship = dynamic_cast<Spaceship*>(args->otherEntity);
    if(spaceship != 0) {
        // old was safely casted to NewType
        spaceship->damage(rand() % 5 + 5);
    }

    if (args->otherEntity) {
        args->otherEntity->applyForce(Vector::byAngle(gameEntity->getAngle() - 90, 600000));
    }

    gameEntity->die();
}

void Spaceship::shootOnce(Point startPoint) {
    Point collisionPoints[] = {
            Point(4, 9),
            Point(6, 9),
            Point(6, 11),
            Point(4, 11)
    };

    GameObject *missile = new GameObject(
            Point(5, 10),
            location + Vector(startPoint.x, startPoint.y),
            textureTurret,
            200,
            10,
            20,
            new CollisionShape(collisionPoints, 4)
    );

    missile->getCollisionEvent()->add(new CollisionEventHandler(onMissileCollision));
    this->getWorld()->addEntity(missile);
    missile->setOwner(this);
    missile->setAngle(angle);
    missile->setSpeed(this->getSpeed());
    missile->applyForce(Vector::byAngle(missile->getAngle() - 90.0, 2000000));
}

void Spaceship::damage(int amount) {
    this->health -= amount;
    if (health <= 0) this->die();
}
