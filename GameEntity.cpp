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

}

void GameEntity::setAngle(double angle) {

}

Point GameEntity::getFocus() {
    return Point(0, 0);
}

void GameEntity::setFocus(Point location) {

}

Point GameEntity::getLocation() {
    return Point(0, 0);
}

void GameEntity::setLocation(Point location) {

}

void GameEntity::turnCounterClockwise() {

}

void GameEntity::turnClockwise() {

}

double GameEntity::getAngle() {
    return 0;
}

GameEntity::GameEntity(Point focus, Point location):focus(focus), location(location) {

}
