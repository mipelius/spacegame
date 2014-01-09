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

GameObjectGroup* createSpaceShip() {

    Texture *textureSpaceShip = new Texture("images/spaceship.png");

    GameObject *objectSpaceShip = new GameObject(
            Point(0, 0),
            Point(0, 0),
            textureSpaceShip,
            nullptr,
            100,
            64,
            64
    );

    GameObjectGroup *spaceShip = new GameObjectGroup(Point(32, 32), Point(4550, 9000));
    spaceShip->add(objectSpaceShip);

    return spaceShip;
}

GameObject* createTurret(double x, double y) {
    if (!textureTurret) textureTurret = new Texture("images/turret.png");

    GameObject *turret = new GameObject(
            Point(8, 32),
            Point(x, y),
            textureTurret,
            nullptr,
            100,
            16,
            32
    );

    return turret;
}

int main(int argc, const char * argv[])
{
    Renderer* renderer = new Renderer();
    renderer->init(0, 0, 1200, 800);
    Map* map = new Map("images/map.bmp");

    GameWorld *world = new GameWorld();
    world->setMap(map);

    renderer->setGameWorld(world);

    GameObjectGroup* spaceShip = createSpaceShip();
    GameObject* leftTurret = createTurret(6, 42);
    leftTurret->setAngle(-7);
    GameObject* rightTurret = createTurret(64-6, 42);
    rightTurret->setAngle(7);

    spaceShip->add(leftTurret);
    spaceShip->add(rightTurret);

    world->addEntity(spaceShip);

    Camera* camera = renderer->getCamera();

    double x = 4000, y = 8500;
    double angle = 0.0;

    int i = 0;
    int addition = 1;

    while (!SDL_QuitRequested()) {
        renderer->render();
        angle += 0.02;

        spaceShip->setLocation(Point(x + (cos(angle) * sin(angle)) * 100 + 500, y + sin(angle) * 100 + 500));

        i += addition;
        if (i > 15) addition = -addition;
        if (i < -15) addition = -addition;
        camera->setLocation(x, y);
        if (i > 0) {
            leftTurret->turnClockwise();
            rightTurret->turnCounterClockwise();
        }
        else {
            leftTurret->turnCounterClockwise();
            rightTurret->turnClockwise();
        }
        spaceShip->turnClockwise();
    }

    delete renderer;
    return 0;
}
