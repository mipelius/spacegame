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

Texture* textureTurret = nullptr;
Texture* textureSpaceShip = nullptr;

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
            40,
            w,
            h
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

    Map* map = new Map("images/map.bmp", mapTexture, 10, 10);

    GameWorld *world = new GameWorld(Vector(0, 9.81), 0.5, 0.005);
    world->setMap(map);

    renderer->setGameWorld(world);

    GameObjectGroup* spaceShip = createSpaceShip();

    GameObject* leftTurret = createTurret(Point(8, 0), Point(6, 30), 16, 32 );
    GameObject* rightTurret = createTurret(Point(8, 0), Point(64-6, 30), 16, 32);
    GameObject* middleTurret = createTurret(Point(6.5, 16), Point(64/2, 0), 13, 32);

    spaceShip->add(leftTurret);
    spaceShip->add(rightTurret);
    spaceShip->add(middleTurret);

    spaceShip->setLocation(Point(4500, 8800));

    world->addEntity(spaceShip);

    Camera* camera = renderer->getCamera();
    //camera->setLocation(0, 7200);

    camera->follow(spaceShip);

    const Uint8* keys;

    while (!SDL_QuitRequested()) {
        keys = SDL_GetKeyboardState(0);

        if (keys[SDL_SCANCODE_LEFT]) spaceShip->setAngle(spaceShip->getAngle() - 5);
        if (keys[SDL_SCANCODE_RIGHT]) spaceShip->setAngle(spaceShip->getAngle() + 5);
        if (keys[SDL_SCANCODE_UP]) spaceShip->applyForce(Vector::byAngle((spaceShip->getAngle() - 90) / 360 * 2 * M_PI, 80000));
        if (keys[SDL_SCANCODE_DOWN]);

        world->step(1.0 / 60.0);
        renderer->render();
    }

    delete renderer;

    return 0;
}