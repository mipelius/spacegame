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
            Point(8, 16),
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
    renderer->init(0, 0, 1200, 800, true);

    std::string filenames[] = {
            "images/red_block.bmp",
            "images/blue_block.bmp",
            "images/green_block.bmp",
    };

    MapTexture *mapTexture = new MapTexture(
            10,
            10,
            3,
            (std::string[]) {
                    "images/red_block.bmp",
                    "images/blue_block.bmp",
                    "images/green_block.bmp"
            }
    );

    Map* map = new Map("images/map.bmp", mapTexture, 10, 10);

    GameWorld *world = new GameWorld();
    world->setMap(map);

    renderer->setGameWorld(world);

    std::list<GameObjectGroup*> *spaceShipList = new std::list<GameObjectGroup*>();

    for(int i=0; i<20; i++) {
        GameObjectGroup* spaceShip = createSpaceShip();
        GameObject* leftTurret = createTurret(6, 42);
        leftTurret->setAngle(-7);
        GameObject* rightTurret = createTurret(64-6, 42);
        rightTurret->setAngle(7);

        spaceShip->add(leftTurret);
        spaceShip->add(rightTurret);
        spaceShip->setLocation(Point(4000 + i * 20, 8500 + i * 20));
        spaceShipList->push_back(spaceShip);

        world->addEntity(spaceShip);

    }

    Camera* camera = renderer->getCamera();
    camera->setLocation(3800, 8400);
    double angle = 0.0;

    int addition = 1;

    while (!SDL_QuitRequested()) {
        renderer->render();
        angle += 0.02;
        int i = 0;
        for (std::list<GameObjectGroup*>::iterator it = spaceShipList->begin(); it != spaceShipList->end(); it++) {
            i++;
            if (i % 3) {
                Point location = (*it)->getLocation();
                (*it)->turnClockwise();
                (*it)->setLocation(Point(location.x + cos(angle) * 2, location.y + sin(angle) * 2));
            }
            else (*it)->turnCounterClockwise();

        }
        Point cameraLocation = camera->getLocation();
        camera->setLocation(cameraLocation.x + cos(angle) * 4, cameraLocation.y + sin(angle) * 4);
    }

    delete renderer;
    return 0;
}
