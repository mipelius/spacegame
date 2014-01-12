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

void onMissileCollision(GameEntity *gameEntity, CollisionEventArgs *args) {
    if (args->map) {
        int x = (int)args->newLocation.x;
        int y = (int)args->newLocation.y;
        for (int i=-50; i<50; i+=10) {
            for (int j=-50; j<50; j+=10) {
                args->map->setValueActual(x + i, y + j, 0);
            }
        }
    }
    gameEntity->die();
}

void shoot(GameWorld* world, Point location, double angle, GameEntity* spaceShip) {
    GameObject *missile = new GameObject(
            Point(10, 20),
            location,
            textureTurret,
            nullptr,
            100,
            20,
            40
    );

    missile->getCollisionEvent()->add(new CollisionEventHandler(onMissileCollision));
    world->addEntity(missile);
    missile->setAngle(angle);
    missile->setSpeed(spaceShip->getSpeed());
    missile->applyForce(Vector::byAngle(missile->getAngle() - 90.0, 1000000));
}

GameObjectGroup* createSpaceShip() {

    if (!textureSpaceShip) textureSpaceShip = new Texture("images/spaceship.png");

    GameObject *objectSpaceShip = new GameObject(
            Point(0, 0),
            Point(0, 0),
            textureSpaceShip,
            nullptr,
            1000,
            64,
            64
    );

    GameObjectGroup *spaceShip = new GameObjectGroup(Point(32, 32), Point(4550, 9000));
    spaceShip->add(objectSpaceShip);

    return spaceShip;
}

GameObject* createTurret(Point focus, Point location, int w, int h) {
    if (!textureTurret) textureTurret = new Texture("images/turret.png");

    GameObject *turret = new GameObject(
            focus,
            location,
            textureTurret,
            nullptr,
            100,
            w,
            h
    );

    return turret;
}

int main(int argc, const char * argv[])
{
    Renderer* renderer = new Renderer();
    renderer->init(0, 0, 1200, 800, true);

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

    Map* map = new Map("images/map.bmp", mapTexture, 10, 10);

    GameWorld *world = new GameWorld(Vector(0, 9.81), 0.25, 0.001);
    world->setMap(map);

    renderer->setGameWorld(world);

    GameObjectGroup* spaceShip = createSpaceShip();

    GameObject* leftTurret = createTurret(Point(8, 0), Point(6, 30), 16, 32);
    GameObject* rightTurret = createTurret(Point(8, 0), Point(64-6, 30), 16, 32);
    GameObject* middleTurret = createTurret(Point(6.5, 16), Point(64/2, 0), 13, 32);

    spaceShip->add(leftTurret);
    spaceShip->add(rightTurret);
    spaceShip->add(middleTurret);

    spaceShip->setLocation(Point(4500, 8800));
    spaceShip->getCollisionEvent()->add(GameWorld::defaultCollisionHandler());

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
            if (deltaTime > 200) {
                shoot(
                        world,
                        spaceShip->getLocation() + Vector::byAngle(spaceShip->getAngle() - 90, 5),
                        spaceShip->getAngle(),
                        spaceShip
                );
                shoot(
                        world,
                        spaceShip->getLocation() + Vector::byAngle(spaceShip->getAngle() + 45, 38),
                        spaceShip->getAngle(),
                        spaceShip
                );
                shoot(
                        world,
                        spaceShip->getLocation() + Vector::byAngle(spaceShip->getAngle() + 180 - 45, 38),
                        spaceShip->getAngle(),
                        spaceShip
                );
                lastMissile = SDL_GetTicks();
            }
        }

        Uint32 timeElapsedMilliSec = SDL_GetTicks() - timeMilliSec;
        world->step(timeElapsedMilliSec / 1000.0);
        timeMilliSec = SDL_GetTicks();
        renderer->render();
    }

    delete renderer;

    return 0;
}