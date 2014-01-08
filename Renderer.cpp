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

void Renderer::init(int x, int y, int w, int h, bool enableFullScreen) {
    // Window initialization
    window = SDL_CreateWindow(
            "",
            x,
            y,
            w,
            h,
            SDL_WINDOW_OPENGL | (enableFullScreen ? SDL_WINDOW_FULLSCREEN : 0)
    );
    if (!window) {
        fprintf(stderr, "Error: %s", SDL_GetError());
        return;
    }

    // GL Rendering context creation
    context = SDL_GL_CreateContext(window);
    if (!context) {
        fprintf(stderr, "Error: %s", SDL_GetError());
        return;
    }

    // GL projection matrix initialization
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, h, 0, -1, 1);
    glClearColor(0.0, 0.0, 0.0, 0.0);

    // cameraInitialization
    this->camera = new Camera(0, 0, w, h);

    isInitialized = true;
}

void Renderer::renderBackground() {

}

void Renderer::renderMap() {
    Map* map = this->gameWorld->getMap();
    Point location = camera->getLocation();
    map->render(location.x, location.y, camera->getW(), camera->getH());
}

void Renderer::renderEntities() {
    std::list<GameEntity*> *gameEntities = gameWorld->getGameEntities();
    Point cameraLocation = camera->getLocation();

    for (std::list<GameEntity*>::iterator it = gameEntities->begin(); it != gameEntities->end(); it++) {
        Point entityLocation = (*it)->getLocation();

        // TODO: render only if the entity is in sight

        (*it)->render(
                (int)(entityLocation.x - cameraLocation.x),
                (int)(entityLocation.y - cameraLocation.y)
        );
    }
}

void Renderer::render() {
    if (!isInitialized) {
        fprintf(stderr, "Error: renderer is not initialized.");
        return;
    }
    glClear(GL_COLOR_BUFFER_BIT);
    renderMap();
    renderEntities();
    SDL_GL_SwapWindow(window);
}

Renderer::Renderer() {
    isInitialized = false;
    this->camera = nullptr;
    this->gameWorld = nullptr;
    this->window = nullptr;
    this->context = nullptr;
}

Renderer::~Renderer() {
    if (context) {
        SDL_GL_DeleteContext(context);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
}

void Renderer::setGameWorld(GameWorld *gameWorld) {
    this->gameWorld = gameWorld;
}

Camera *Renderer::getCamera() {
    if (!isInitialized) {
        fprintf(stderr, "Error: renderer is not initialized.");
        return nullptr;
    }
    return this->camera;
}
