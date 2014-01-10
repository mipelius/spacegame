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

    // create list of backgrounds
    this->backgrounds = new std::list<Background*>();

    isInitialized = true;
}

void Renderer::renderBackground() {
    Point cameraLocation = camera->getLocation();
    for (std::list<Background*>::iterator it = backgrounds->begin(); it != backgrounds->end(); it++) {
        if (
                cameraLocation.x >= (*it)->getX() &&
                cameraLocation.x < (*it)->getX() + (*it)->getW() &&
                cameraLocation.y >= (*it)->getY() &&
                cameraLocation.y < (*it)->getY() + (*it)->getH()
                ) {

            double ratioX = ((*it)->getW() - camera->getW()) / (*it)->getW();
            double ratioY = ((*it)->getH() - camera->getH()) / (*it)->getH();
            int offSetX = (int)((cameraLocation.x - (*it)->getX()) * ratioX);
            int offSetY = (int)((cameraLocation.y - (*it)->getY()) * ratioY);

            (*it)->render(
                    offSetX,
                    offSetY,
                    (int)this->camera->getW(),
                    (int)this->camera->getH()
            );
        }

    }
}

void Renderer::addBackground(Background *background) {
    backgrounds->push_back(background);
}

void Renderer::renderMap() {
    Map* map = this->gameWorld->getMap();
    Point location = camera->getLocation();
    map->render(
            (int)location.x,
            (int)location.y,
            (int)camera->getW(),
            (int)camera->getH()
    );
}

void Renderer::renderEntities() {
    std::list<GameEntity*> *gameEntities = gameWorld->getGameEntities();
    Point cameraLocation = camera->getLocation();

    glMatrixMode(GL_MODELVIEW);

    for (std::list<GameEntity*>::iterator it = gameEntities->begin(); it != gameEntities->end(); it++) {
        Point entityLocation = (*it)->getLocation();

        // TODO: render only if the entity is in sight

        (*it)->render(
                (int)(entityLocation.x - cameraLocation.x),
                (int)(entityLocation.y - cameraLocation.y)
        );
    }

    glLoadIdentity();
}

void Renderer::render() {
    if (!isInitialized) {
        fprintf(stderr, "Error: renderer is not initialized.");
        return;
    }
    glClear(GL_COLOR_BUFFER_BIT);
    renderBackground();
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
