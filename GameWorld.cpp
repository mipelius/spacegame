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

#include "GameWorld.h"

void GameWorld::setMap(Map* map) {
    this->map = map;
}

void GameWorld::addEntity(GameEntity *gameEntity) {
    gameEntities->push_back(gameEntity);
}

void GameWorld::step(double timeSeconds) {
    for(std::list<GameEntity*>::iterator it = gameEntities->begin(); it != gameEntities->end(); it++) {
        Vector airResistance = (*it)->getSpeed() * (*it)->getSpeed() * (0.5 * airThickness);
        Vector acceleration = ((*it)->getForce() - airResistance) * (1 / (*it)->getMass());

        acceleration += gForce;
        (*it)->setSpeed((*it)->getSpeed() + acceleration);
        (*it)->setLocation((*it)->getLocation() + (*it)->getSpeed() * timeSeconds * metersPerPixel);
        (*it)->setForceToZero();
    }
}

GameWorld::GameWorld(
        Vector gForce,
        double metersPerPixel,
        double airThickness
): gForce(gForce), metersPerPixel(metersPerPixel), airThickness(airThickness) {
    this->map = nullptr;
    this->gameEntities = new std::list<GameEntity*>();
}

Map *GameWorld::getMap() {
    return this->map;
}

std::list<GameEntity*> *GameWorld::getGameEntities() {
    return this->gameEntities;
}

long GameWorld::getW() {
    return this->map->getActualW();
}

long GameWorld::getH() {
    return this->map->getActualH();
}
