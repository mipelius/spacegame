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
#include "Renderer.h"
#include "Camera.h"
#include "Background.h"
#include "GameWorld.h"
#include "Map.h"
#include "CollisionShape.h"
#include "GameEntity.h"

void Renderer::init(int x, int y, int w, int h, bool enableFullScreen) {
    // Window initialization
    window_ = SDL_CreateWindow(
            "",
            x,
            y,
            w,
            h,
            SDL_WINDOW_OPENGL | (enableFullScreen ? SDL_WINDOW_FULLSCREEN : 0)
    );
    if (!window_) {
        std::fprintf(stderr, "Error: %s", SDL_GetError());
        return;
    }

    // GL Rendering context creation
    context_ = SDL_GL_CreateContext(window_);
    if (!context_) {
        fprintf(stderr, "Error: %s", SDL_GetError());
        return;
    }

    // GL projection matrix initialization
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, h, 0, -1, 1);
    glClearColor(0.0, 0.0, 0.0, 0.0);

    // cameraInitialization
    this->camera_ = new Camera(0, 0, w, h, this);

    // background initialization
    background_ = nullptr;
    oldBackground_ = nullptr;

    isInitialized_ = true;

    collisionShapesAreVisible_ = false;
    mapCollisionAreaIsVisible_ = false;
}

void Renderer::renderBackground() {
    if (backgroundIsCrossFading) {
        double opacity = (double)backgroundCrossFadingPhase / (double)BACKGROUND_CROSSFADE_RENDER_COUNT;

        renderBackground(background_, opacity);
        renderBackground(oldBackground_, 1.0 - opacity);

        backgroundCrossFadingPhase++;
        if (backgroundCrossFadingPhase > BACKGROUND_CROSSFADE_RENDER_COUNT) {
            backgroundCrossFadingPhase = 0;
            backgroundIsCrossFading = false;
        }
    }
    else renderBackground(background_, 1.0);
}

void Renderer::setBackground(Background *background) {
    oldBackground_ = background_;
    background_ = background;
    backgroundIsCrossFading = true;
    backgroundCrossFadingPhase = 0;
}

void Renderer::renderMap() {
    Map* map = this->gameWorld_->getMap();
    Point location = camera_->getLocation();

    map->render(
            location.x,
            location.y,
            camera_->getW(),
            camera_->getH()
    );

    Point cameraCenter = Point(location.x + camera_->getW() / 2, location.y + camera_->getH() / 2);

    double x1 = cameraCenter.x - smallMapWidth / 2;
    double y1 = cameraCenter.y - smallMapHeight / 2;
    double x2 = cameraCenter.x + smallMapWidth / 2;
    double y2 = cameraCenter.y + smallMapHeight / 2;

    map->renderSmall(
            Rect(Point(x1, y1), Point(x2, y2)),
            smallMapRenderingAreaRect
    );

}

void Renderer::renderEntities() {
    std::list<GameEntity*> *gameEntities = gameWorld_->getGameEntities();
    Point cameraLocation = camera_->getLocation();

    glMatrixMode(GL_MODELVIEW);

    for (std::list<GameEntity*>::iterator it = gameEntities->begin(); it != gameEntities->end(); it++) {
        Point location = (*it)->getLocation();

        (*it)->render(
                location.x - cameraLocation.x,
                location.y - cameraLocation.y
        );

        CollisionShape* shape = (*it)->getCollisionShape();
        Point* points = shape->getRotatedPoints();

        if (collisionShapesAreVisible_) {
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

        if (mapCollisionAreaIsVisible_) {

			Rect boundingBox = (*it)->getCollisionShape()->getBoundingBox();

			int w = this->gameWorld_->getMap()->getBlockW();
			int h = this->gameWorld_->getMap()->getBlockH();

			int iBegin = (int)boundingBox.getFirstPoint().x - ((int)boundingBox.getFirstPoint().x) % w;
			int iEnd = (int)boundingBox.getSecondPoint().x + (int)boundingBox.getSecondPoint().x % w;
			int jBegin = (int)boundingBox.getFirstPoint().y - ((int)boundingBox.getFirstPoint().y) % h;
			int jEnd = (int)boundingBox.getSecondPoint().y + (int)boundingBox.getSecondPoint().y % h;

			glDisable(GL_TEXTURE_2D);

			for (int i=iBegin; i <= iEnd; i += w) {
				for (int j=jBegin; j <= jEnd ; j += h) {
					if (gameWorld_->getMap()->getValueActual(i, j)) glColor3f(1, 1, 1);
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
    if (!isInitialized_) {
        fprintf(stderr, "Error: renderer is not initialized.");
        return;
    }
    glClear(GL_COLOR_BUFFER_BIT);
    renderBackground();
    renderMap();
    renderEntities();
}

Renderer::Renderer() : smallMapRenderingAreaRect(Point(10, 10), Point(250, 250)) {
    isInitialized_ = false;
    this->camera_ = nullptr;
    this->gameWorld_ = nullptr;
    this->window_ = nullptr;
    this->context_ = nullptr;
}

Renderer::~Renderer() {
    if (context_) {
        SDL_GL_DeleteContext(context_);
    }
    if (window_) {
        SDL_DestroyWindow(window_);
    }
}

GameWorld* Renderer::getGameWorld() {
    return this->gameWorld_;
}

void Renderer::setGameWorld(GameWorld *gameWorld) {
    this->gameWorld_ = gameWorld;
}

Camera *Renderer::getCamera() {
    if (!isInitialized_) {
        fprintf(stderr, "Error: renderer is not initialized.");
        return nullptr;
    }
    return this->camera_;
}

void Renderer::showCollisionShapes() {
    collisionShapesAreVisible_ = true;
}

void Renderer::hideCollisionShapes() {
    collisionShapesAreVisible_ = false;
}

void Renderer::toggleCollisionShapesVisibility() {
    collisionShapesAreVisible_ = !collisionShapesAreVisible_;
}

void Renderer::glSwap() {
    SDL_GL_SwapWindow(window_);
}

void Renderer::toggleMapCollisionAreaVisibility() {
    mapCollisionAreaIsVisible_ = !mapCollisionAreaIsVisible_;
}

void Renderer::hideMapCollisionArea() {
    mapCollisionAreaIsVisible_ = false;
}

void Renderer::showMapCollisionArea() {
    mapCollisionAreaIsVisible_ = true;
}

void Renderer::renderBackground(Background *background, double opacity) {
    if (background == nullptr) return;

    Point location = camera_->getLocation();

    // TODO: when going over the bounds make automatic mixing between backgrounds top and bottom / left and right

    double ratioX = (background->getW() - camera_->getW()) / background->getW();
    double ratioY = (background->getH() - camera_->getH()) / background->getH();
    int offSetX = (int)((location.x - background->getX()) * ratioX);
    int offSetY = (int)((location.y - background->getY()) * ratioY);

    background->render(
            offSetX,
            offSetY,
            (int)this->camera_->getW(),
            (int)this->camera_->getH(),
            opacity
    );

}

void Renderer::setSmallMap(Rect renderingAreaRect, double mapWidth, double mapHeight) {
    this->smallMapRenderingAreaRect = renderingAreaRect;
    this->smallMapWidth = mapWidth;
    this->smallMapHeight = mapHeight;
}
