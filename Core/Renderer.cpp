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
        std::fprintf(stderr, "Error: %s", SDL_GetError());
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
    this->camera = new Camera(0, 0, w, h, this);

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
            location.x,
            location.y,
            camera->getW(),
            camera->getH()
    );
}

void Renderer::renderEntities() {
    std::list<GameEntity*> *gameEntities = gameWorld->getGameEntities();
    Point cameraLocation = camera->getLocation();

    glMatrixMode(GL_MODELVIEW);

    for (std::list<GameEntity*>::iterator it = gameEntities->begin(); it != gameEntities->end(); it++) {
        Point entityLocation = (*it)->getLocation();

        (*it)->render(
                entityLocation.x - cameraLocation.x,
                entityLocation.y - cameraLocation.y
        );

        CollisionShape* shape = (*it)->getCollisionShape();
        Point* points = shape->getRotatedPoints();
        Point location = shape->getLocation();

        if (collisionShapesAreVisible) {
            // collision shape

            glColor3f(1, 1, 1);
            glDisable(GL_TEXTURE_2D);

            glBegin(GL_LINE_LOOP);
            for (int i=0; i<shape->getCount(); i++) {
                glVertex2f(
                        points[i].x + location.x - cameraLocation.x,
                        points[i].y + location.y - cameraLocation.y
                );
            }
            glEnd();

            // bounding box

            glColor3f(0.2, 0.2, 0.2);

            Rect rect = shape->getBoundingBox();

            glBegin(GL_LINE_LOOP);
            glVertex2f(rect.getTopLeftCorner().x - cameraLocation.x, rect.getTopLeftCorner().y - cameraLocation.y);
            glVertex2f(rect.getTopRightCorner().x - cameraLocation.x, rect.getTopRightCorner().y - cameraLocation.y);
            glVertex2f(rect.getBottomRightCorner().x - cameraLocation.x, rect.getBottomRightCorner().y - cameraLocation.y);
            glVertex2f(rect.getBottomLeftCorner().x - cameraLocation.x, rect.getBottomLeftCorner().y - cameraLocation.y);
            glEnd();
        }

        if (mapCollisionAreaIsVisible) {

        Rect boundingBox = (*it)->getCollisionShape()->getBoundingBox();

        int w = this->gameWorld->getMap()->getBlockW();
        int h = this->gameWorld->getMap()->getBlockH();

        int iBegin = (int)boundingBox.getFirstPoint().x - ((int)boundingBox.getFirstPoint().x) % w;
        int iEnd = (int)boundingBox.getSecondPoint().x + (int)boundingBox.getSecondPoint().x % w;
        int jBegin = (int)boundingBox.getFirstPoint().y - ((int)boundingBox.getFirstPoint().y) % h;
        int jEnd = (int)boundingBox.getSecondPoint().y + (int)boundingBox.getSecondPoint().y % h;

        glDisable(GL_TEXTURE_2D);

        for (int i=iBegin; i <= iEnd; i += w) {
            for (int j=jBegin; j <= jEnd ; j += h) {
                if (gameWorld->getMap()->getValueActual(i, j)) glColor3f(1, 1, 1);
                else glColor3f(0.2, 0, 0);

                Rect rect = Rect(Point(i, j), Point(i + w, j + h));
                glBegin(GL_LINE_LOOP);
                glVertex2f(rect.getTopLeftCorner().x - cameraLocation.x, rect.getTopLeftCorner().y - cameraLocation.y);
                glVertex2f(rect.getTopRightCorner().x - cameraLocation.x, rect.getTopRightCorner().y - cameraLocation.y);
                glVertex2f(rect.getBottomRightCorner().x - cameraLocation.x, rect.getBottomRightCorner().y - cameraLocation.y);
                glVertex2f(rect.getBottomLeftCorner().x - cameraLocation.x, rect.getBottomLeftCorner().y - cameraLocation.y);
                glEnd();

            }
        }
        }
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

GameWorld* Renderer::getGameWorld() {
    return this->gameWorld;
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

void Renderer::showCollisionShapes() {
    collisionShapesAreVisible = true;
}

void Renderer::hideCollisionShapes() {
    collisionShapesAreVisible = false;
}

void Renderer::toggleCollisionShapesVisibility() {
    collisionShapesAreVisible = !collisionShapesAreVisible;
}

void Renderer::glSwap() {
    SDL_GL_SwapWindow(window);
}

void Renderer::toggleMapCollisionAreaVisibility() {
    mapCollisionAreaIsVisible = !mapCollisionAreaIsVisible;
}

void Renderer::hideMapCollisionArea() {
    mapCollisionAreaIsVisible = false;
}

void Renderer::showMapCollisionArea() {
    mapCollisionAreaIsVisible = true;
}
