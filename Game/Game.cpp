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
#include "Game.h"
#include "App.h"
#include "Window.h"
#include "Canvas.h"

#include "Map.h"
#include "Body.h"
#include "PhysicsWorld.h"

#include "DrawableObject.h"
#include "DrawableMap.h"
#include "MapTexture.h"
#include "Camera.h"

Game::Game() {

    // --- WINDOW PROPERTIES ---

    int x = 0;
    int y = 0;
    int w = 1280;
    int h = 800;

    bool enableFullScreen = false;

    Window* window = App::getInstance()->getWindow();
    window->initialize(x, y, w, h, enableFullScreen);

    // --- CANVAS ---

    Canvas* canvas = new Canvas();
    canvas->setMargin(0, 0, 0, 0);
    canvas->setAnchor(GuiComponentBase::Anchor::BOTTOM_RIGHT);

    window->addComponent(canvas);

    // --- WORLD & MAP ---

    map_ = new Map("images/map.bmp", 10, 10);
    world_ = new PhysicsWorld(Vector(0, 9.81), 0.20, 0.0001);
    world_->setMap(map_);

    DrawableMap* drawableMap = new DrawableMap();
    drawableMap->setMap(map_);

    std::string filenames[] = {
            "images/green_block.bmp",
            "images/red_block.bmp",
            "images/blue_block.bmp",
    };

    MapTexture *mapTexture = new MapTexture(10, 10,	3, filenames);
    drawableMap->setMapTexture(mapTexture);

    camera_ = new Camera();
    camera_->boundsRect->set(Rect(0, 0, 20000, 20000));
    camera_->areaRect->set(canvas->getRenderingAreaRect());

    canvas->add(drawableMap);
    canvas->setCamera(camera_);

    // --- PLAYER ---

    Body* body = new Body(200);
    body->location->set(Point(10000, 10000));
    world_->add(body);

    camera_->location->bind(body->location);
}

void Game::launch() {
    const Uint8* keys;
    Uint32 timeMilliSec = 0;

    while (!SDL_QuitRequested()) {
        /// --- INPUT READING AND HANDLING ---

        keys = SDL_GetKeyboardState(0);

        if (keys[SDL_SCANCODE_LEFT]);
        if (keys[SDL_SCANCODE_RIGHT]);
        if (keys[SDL_SCANCODE_UP]);
        if (keys[SDL_SCANCODE_SPACE]);

        /// --- PHYSICS ---

        Uint32 timeElapsedMilliSec = SDL_GetTicks() - timeMilliSec;
        world_->step(timeElapsedMilliSec / 1000.0);
        timeMilliSec = SDL_GetTicks();

        /// --- RENDERING ---

        App::getInstance()->getWindow()->update();
    }
}

Game::~Game() {
    delete map_;
    delete world_;
}
