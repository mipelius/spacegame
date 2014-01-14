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

#include <iostream>
#include "Renderer.h"
#include "GameObjectGroup.h"
#include "CollisionEventArgs.h"
#include "CollisionEventHandler.h"

Texture* textureTurret = nullptr;
Texture* textureSpaceShip = nullptr;

GameObjectGroup* spaceShip = nullptr;

void onSpaceshipCollision(GameEntity *entity, CollisionEventArgs* args) {
    if (args->otherEntity) return;
    entity->setSpeed(entity->getSpeed() * 0.0);
    entity->setLocation(args->oldLocation);
}

void onMissileCollision(GameEntity *gameEntity, CollisionEventArgs *args) {
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
    if (args->otherEntity && args->otherEntity != spaceShip) {
        args->otherEntity->applyForce(Vector::byAngle(gameEntity->getAngle() - 90, 600000));

    }
    if (args->otherEntity != spaceShip) gameEntity->die();
}

void shoot(GameWorld* world, Point location, double angle, GameEntity* spaceShip) {
    Point collisionPoints[] = {
            Point(0, 5),
            Point(10, 5),
            Point(10, 15),
            Point(0, 15)
    };

    GameObject *missile = new GameObject(
            Point(5, 10),
            location,
            textureTurret,
            200,
            10,
            20,
            new CollisionShape(collisionPoints, 4)
    );

    missile->getCollisionEvent()->add(new CollisionEventHandler(onMissileCollision));
    world->addEntity(missile);
    missile->setAngle(angle);
    missile->setSpeed(spaceShip->getSpeed());
    missile->applyForce(Vector::byAngle(missile->getAngle() - 90.0, 2000000));
}

GameObjectGroup* createSpaceShip() {

    if (!textureSpaceShip) textureSpaceShip = new Texture("images/spaceship.png");

    GameObject *objectSpaceShip = new GameObject(
            Point(0, 0),
            Point(0, 0),
            textureSpaceShip,
            1000,
            32,
            32,
            nullptr
    );

    Point collisionPoints[] = {
            Point(16, 0),
            Point(32, 32),
            Point(0, 32)
    };

    GameObjectGroup *spaceShip = new GameObjectGroup(
            Point(16, 16), Point(4550, 9000),
            new CollisionShape(collisionPoints, 3)
    );
    spaceShip->add(objectSpaceShip);

    return spaceShip;
}

GameObject* createTurret(Point focus, Point location, int w, int h) {
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

int main(int argc, const char * argv[])
{
    Renderer* renderer = new Renderer();
    renderer->init(0, 0, 1920, 1200, true);

    renderer->addBackground(
            new Background(
                    new Texture("images/bg1.jpg"),
                    0,
                    7200
            )
    );

    renderer->addBackground(
            new Background(
                    new Texture("images/bg2.jpg"),
                    3400,
                    7200
            )
    );

    MapTexture *mapTexture = new MapTexture(
            10,
            10,
            3,
            (std::string[]) {
                    "images/green_block.bmp",
                    "images/red_block.bmp",
                    "images/blue_block.bmp",
            }
    );

    Map* map = new Map("images/map.bmp", mapTexture, 5, 5);

    GameWorld *world = new GameWorld(Vector(0, 9.81), 0.15, 0.001);
    world->setMap(map);


    for (int i=0; i<20; i++) {
        GameObjectGroup* enemy = createSpaceShip();
        enemy->setLocation(Point(4500 + i * 50, 8700));
        enemy->getCollisionEvent()->add(new CollisionEventHandler(onSpaceshipCollision));
        world->addEntity(enemy);
    }

    renderer->setGameWorld(world);

    spaceShip = createSpaceShip();

    GameObject* leftTurret = createTurret(Point(4, 0), Point(3, 10), 8, 16);
    GameObject* rightTurret = createTurret(Point(4, 0), Point(32-3, 10), 8, 16);
    GameObject* middleTurret = createTurret(Point(2.5, 5), Point(32/2, 0), 5, 10);

    spaceShip->add(leftTurret);
    spaceShip->add(rightTurret);
    spaceShip->add(middleTurret);

    spaceShip->setLocation(Point(4500, 8800));
    spaceShip->getCollisionEvent()->add(new CollisionEventHandler(onSpaceshipCollision));

    world->addEntity(spaceShip);

    Camera* camera = renderer->getCamera();

    camera->follow(spaceShip);

    const Uint8* keys;

    Uint32 timeMilliSec = SDL_GetTicks();
    Uint32 lastMissile = 0;

    while (!SDL_QuitRequested()) {
        keys = SDL_GetKeyboardState(0);

        if (keys[SDL_SCANCODE_LEFT]) spaceShip->setAngle(spaceShip->getAngle() - 5);
        if (keys[SDL_SCANCODE_RIGHT]) spaceShip->setAngle(spaceShip->getAngle() + 5);
        if (keys[SDL_SCANCODE_UP]) spaceShip->applyForce(Vector::byAngle(spaceShip->getAngle() - 90, 120000));
        if (keys[SDL_SCANCODE_SPACE]) {
            Uint32 deltaTime = SDL_GetTicks() - lastMissile;
            if (deltaTime > 100) {
                shoot(
                        world,
                        spaceShip->getLocation() + Vector::byAngle(spaceShip->getAngle() - 90, 5),
                        spaceShip->getAngle(),
                        spaceShip
                );
                shoot(
                        world,
                        spaceShip->getLocation() + Vector::byAngle(spaceShip->getAngle() + 45, 18),
                        spaceShip->getAngle(),
                        spaceShip
                );
                shoot(
                        world,
                        spaceShip->getLocation() + Vector::byAngle(spaceShip->getAngle() + 180 - 45, 18),
                        spaceShip->getAngle(),
                        spaceShip
                );
                lastMissile = SDL_GetTicks();
            }
        }

        if (keys[SDL_SCANCODE_F1]) renderer->toggleCollisionShapesVisibility();

        Uint32 timeElapsedMilliSec = SDL_GetTicks() - timeMilliSec;
        world->step(timeElapsedMilliSec / 1000.0);
        timeMilliSec = SDL_GetTicks();
        renderer->render();
    }

    delete renderer;

    return 0;
}