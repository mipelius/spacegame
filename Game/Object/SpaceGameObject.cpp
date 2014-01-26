// This file is part of SpaceGame.
// Copyright (C) 2014 Miika Pelkonen
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
#include "SpaceGameObject.h"
#include "Controller.h"
#include "Team.h"

SpaceGameObject::SpaceGameObject(Point location, double angle, CollisionShape *shape, int maxHealth) :
GameObjectGroup(location, angle, shape) {
    this->health = maxHealth;
    this->maxHealth = maxHealth;
    this->controller = nullptr;
    this->_team = nullptr;
}

void SpaceGameObject::damage(int damage) {
    health -= damage;
    if (health <= 0) this->die();
}

int SpaceGameObject::getHealth() {
    return health;
}

int SpaceGameObject::getMaxHealth() {
    return maxHealth;
}

void SpaceGameObject::beforeStep(double timeElapsedSec) {
    GameEntity::beforeStep(timeElapsedSec);
    if (controller) controller->control(timeElapsedSec);
}

void SpaceGameObject::setTeam(Team *team) {
    if (_team) _team->members->remove(this);
    team->members->push_back(this);
    this->_team = team;
}

Team *SpaceGameObject::getTeam() {
    return _team;
}

void SpaceGameObject::beforeEntityCollisionDetection(GameEntity *otherEntity) {
    SpaceGameObject* spaceGameObject = dynamic_cast<SpaceGameObject*>(otherEntity);

    if (spaceGameObject) {
        if (spaceGameObject->getTeam() == this->_team) ignoreEntityCollisionDetection();
    }
}

void SpaceGameObject::onDying() {
    GameEntity::onDying();
    if (_team) _team->members->remove(this);
    //delete this;
}