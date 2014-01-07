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

const int BLOCK_SIZE_W = 8;
const int BLOCK_SIZE_H = 8;

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

    int iStart = 0;
    int jStart = 0;

    Point cameraLocation = camera->getLocation();

    if (cameraLocation.x > 0) iStart = (int)(cameraLocation.x / BLOCK_SIZE_W);
    if (cameraLocation.y > 0) jStart = (int)(cameraLocation.y / BLOCK_SIZE_H);

    if (iStart < 0) iStart = 0;
    if (jStart < 0) jStart = 0;

    int iEnd = map->getW();
    int jEnd = map->getH();

    if (cameraLocation.x + camera->getW() < map->getW() * BLOCK_SIZE_W) iEnd = (int)((cameraLocation.x + camera->getW()) / BLOCK_SIZE_W + 1);
    if (cameraLocation.y + camera->getH() < map->getH() * BLOCK_SIZE_H) jEnd = (int)((cameraLocation.y + camera->getH()) / BLOCK_SIZE_H + 1);

    // TODO: use Block-object instead of unsigned char

    for (int i = iStart; i < iEnd; i++) {
        for (int j = jStart; j < jEnd; j++) {
            if (!map->getValue(i, j)) continue; // continue if the block is empty
            glColor3f(1.0, 1.0, 1.0);

            glBegin(GL_QUADS);

            //Bottom-left vertex (corner)
            glVertex3f(i * BLOCK_SIZE_W - cameraLocation.x + 0.f, j * BLOCK_SIZE_H - cameraLocation.y + 0.f, 0.0f);

            //Bottom-right vertex (corner)
            glVertex3f(i * BLOCK_SIZE_W - cameraLocation.x + BLOCK_SIZE_W, j * BLOCK_SIZE_H - cameraLocation.y + 0.f, 0.f);

            //Top-right vertex (corner)
            glVertex3f(i * BLOCK_SIZE_W - cameraLocation.x + BLOCK_SIZE_W, j * BLOCK_SIZE_H - cameraLocation.y + BLOCK_SIZE_H, 0.f);

            //Top-left vertex (corner)
            glVertex3f(i * BLOCK_SIZE_W - cameraLocation.x + 0.f, j * BLOCK_SIZE_H - cameraLocation.y + BLOCK_SIZE_H, 0.f);

            glEnd();
        }
    }
    glColor3d(1, 1, 1);
}

void Renderer::renderEntities() {
    std::list<GameEntity*> *gameEntities = gameWorld->getGameEntities();
    Point cameraLocation = camera->getLocation();

    for (std::list<GameEntity*>::iterator it = gameEntities->begin(); it != gameEntities->end(); it++) {
        Point entityLocation = (*it)->getLocation();

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
