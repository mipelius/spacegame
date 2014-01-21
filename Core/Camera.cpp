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
#include "Camera.h"
#include "Renderer.h"

Camera::Camera(double x, double y, double w, double h, Renderer *renderer): x(x), y(y), w(w), h(h)  {
    entityToFollow = nullptr;
    this->renderer = renderer;
}

Point Camera::getLocation() {
    if (entityToFollow) {
        Point entityLocation = entityToFollow->getLocation();
        this->setLocation(entityLocation.x - w/2, entityLocation.y - h/2);
    }
    return Point(x, y);
}

void Camera::follow(GameEntity *gameEntity) {
    entityToFollow = gameEntity;
}

double Camera::getW() {
    return this->w;
}

double Camera::getH() {
    return this->h;
}

void Camera::setLocation(double x, double y) {
    GameWorld *gameWorld = this->renderer->getGameWorld();
    this->x = x;
    this->y = y;
    if (this->x < 0) this->x = 0;
    if (this->x + w > gameWorld->getW()) this->x = gameWorld->getW() - w;
    if (this->y < 0) this->y = 0;
    if (this->y + h > gameWorld->getH()) this->y = gameWorld->getH() - h;
}
