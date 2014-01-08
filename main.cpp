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

int main(int argc, const char * argv[])
{
    Renderer* renderer = new Renderer();
    renderer->init(0, 0, 1200, 800);
    Map* map = new Map("images/map.bmp");

    GameWorld *world = new GameWorld();
    world->setMap(map);

    renderer->setGameWorld(world);

    GameObject *object = new GameObject(
            Point(50, 50),
            Point(4500, 9000),
            nullptr,
            nullptr,
            100,
            100,
            100
    );

    world->addObject(object);

    Camera* camera = renderer->getCamera();

    double x = 4000, y = 8500;
    double angle = 0.0;

    while (!SDL_QuitRequested()) {
        renderer->render();
        angle += 0.01;
        x += cos(angle) * 1;
        y += sin(angle) * 1;
        camera->setLocation(x, y);
        object->turnCounterClockwise();
    }

    delete renderer;
    return 0;
}
