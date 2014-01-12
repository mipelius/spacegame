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
#include "GameWorld.h"
#include "CollisionEventArgs.h"
#include "CollisionEventHandler.h"

void GameWorld::setMap(Map* map) {
    this->map = map;
}

void GameWorld::addEntity(GameEntity *gameEntity) {
    gameEntities->push_back(gameEntity);
}

void GameWorld::step(double timeSeconds) {
    for(std::list<GameEntity*>::iterator it = gameEntities->begin(); it != gameEntities->end(); it++) {
        Vector airResistance = (*it)->getSpeed() * (*it)->getSpeed() * (0.5 * airDensity);
        Vector acceleration = ((*it)->getForce() - airResistance) * (1 / (*it)->getMass());

        acceleration += gForce;
        (*it)->setSpeed((*it)->getSpeed() + acceleration);
        Point oldLocation = (*it)->getLocation();
        Point newLocation = oldLocation + (*it)->getSpeed() * timeSeconds * metersPerPixel;
        (*it)->setForceToZero();
        (*it)->setLocation(newLocation);

        detectCollision((*it), oldLocation, newLocation);
    }
}

GameWorld::GameWorld(
        Vector gForce,
        double metersPerPixel,
        double airDensity
): gForce(gForce), metersPerPixel(metersPerPixel), airDensity(airDensity) {
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

void GameWorld::detectCollision(GameEntity *entity, Point oldLocation, Point newLocation) {
    bool isMapCollision = map->getValueActual((int)newLocation.x, (int)newLocation.y);
    bool isBoundsCollision = newLocation.x < 0 || newLocation.x > getW() || newLocation.y < 0 || newLocation.y > getH();
    // bool isEntityCollision = ???

    if (isMapCollision || isBoundsCollision /* || isEntityCollision */) {
        CollisionEventArgs* args = new CollisionEventArgs();
        if (isMapCollision) args->map = this->map;
        args->oldLocation = oldLocation;
        args->newLocation = newLocation;
        args->collisionLocation = newLocation;
        entity->getCollisionEvent()->raise(args);
        delete args;
    }
}

void GameWorld::defaultOnEntityCollision(GameEntity *entity, CollisionEventArgs* args) {
    entity->setSpeed(entity->getSpeed() * -0.2);
    entity->setLocation(args->oldLocation);
}

CollisionEventHandler *GameWorld::defaultCollisionHandler() {
    return new CollisionEventHandler(defaultOnEntityCollision);
}
