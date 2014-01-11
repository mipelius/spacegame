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

#include "GameEntity.h"

void GameEntity::applyForce(Vector force) {
    this->force = force;
}

void GameEntity::setAngle(double angle) {
    this->angle = angle;
}

Point GameEntity::getFocus() {
    return this->focus;
}

void GameEntity::setFocus(Point focus) {
    this->focus = focus;
}

Point GameEntity::getLocation() {
    return this->location;
}

void GameEntity::setLocation(Point location) {
    this->location = location;
}

void GameEntity::turnCounterClockwise() {
    angle--;
}

void GameEntity::turnClockwise() {
    angle++;
}

double GameEntity::getAngle() {
    return this->angle;
}

GameEntity::GameEntity(Point focus, Point location) :
    focus(focus), location(location), force(Vector(0, 0)), speed(Vector(0, 0)) {
    this->angle = 0;
}

void GameEntity::render(int x, int y) {

}

double GameEntity::getMass() {
    return 0.0;
}

Vector GameEntity::getForce() {
    return this->force;
}

void GameEntity::setSpeed(Vector speed) {
    this->speed = speed;
}

Vector GameEntity::getSpeed() {
    return speed;
}

void GameEntity::setForceToZero() {
    this->force = Vector(0, 0);
}
