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
#include "GameWorld.h"
#include "Event.h"
#include "CollisionEventArgs.h"
#include "CollisionEventHandler.h"
#include "GameEntity.h"
#include "Map.h"

void GameWorld::setMap(Map* map) {
    this->map = map;
}

void GameWorld::addEntity(GameEntity *gameEntity) {
    gameEntities->push_back(gameEntity);
    gameEntity->setWorld(this);
}

void GameWorld::step(double timeSeconds) {
    // update velocities and new locations
    for(std::list<GameEntity*>::iterator it = gameEntities->begin(); it != gameEntities->end(); it++) {
        (*it)->step(timeSeconds);
        // compute total force

    }

    // now all the new locations are updated -> detect collision
    for(std::list<GameEntity*>::iterator it = gameEntities->begin(); it != gameEntities->end(); it++) {
        detectCollision((*it), (*it)->getLocationBeforeUpdate(), (*it)->getLocation());
    }

    // remove dead entities from world
    for (std::list<GameEntity*>::iterator it = gameEntities->begin(); it != gameEntities->end(); it++) {
        if ((*it)->isDead()) {
			std::list<GameEntity*>::iterator removedIterator = it;
            GameEntity* currentEntity = (*it);
			it++;
			gameEntities->erase(removedIterator);
            currentEntity->setWorld(nullptr);
        }
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
    bool isBoundsCollision = newLocation.x < 0 || newLocation.x > getW() || newLocation.y < 0 || newLocation.y > getH();
    bool isMapCollision = map->detectCollisionWith(entity);
    bool isEntityCollision = false;

    GameEntity* otherEntity = nullptr;

    for (std::list<GameEntity*>::iterator it = gameEntities->begin(); it != gameEntities->end(); it++) {
        if ((*it) == entity) continue;
        if (entity->_detectCollisionWith(*it)) {
            otherEntity = (*it);
            isEntityCollision = true;
        };
        if (otherEntity) break;
    }

    if (isMapCollision || isBoundsCollision || isEntityCollision) {
        CollisionEventArgs* args = new CollisionEventArgs();
        if (isMapCollision) args->map = this->map;
        if (isEntityCollision) args->otherEntity = otherEntity;
        args->oldLocation = oldLocation;
        args->newLocation = newLocation;
        args->collisionLocation = newLocation;
        entity->getCollisionEvent()->raise(args);
        delete args;
    }
}

const Vector GameWorld::getGForce() {
    return Vector(gForce.x, gForce.y);
}

double GameWorld::getMetersPerPixel() {
    return metersPerPixel;
}

double GameWorld::getAirDensity() {
    return airDensity;
}
