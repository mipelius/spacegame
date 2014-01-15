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
#include "GameEntity.h"

void GameEntity::applyForce(Vector force) {
    this->force += force;
}

void GameEntity::setAngle(double angle) {
    if (this->collisionShape != nullptr) this->collisionShape->setAngle(this->angle);
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
    if (this->collisionShape != nullptr) this->collisionShape->setLocation(location - Vector(this->focus.x, this->focus.y));
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

GameEntity::GameEntity(Point focus, Point location, CollisionShape* collisionShape) :
    focus(focus),
    location(location),
    force(Vector(0, 0)),
    speed(Vector(0, 0)),
    collisionEvent(new Event(this)),
    collisionShape(collisionShape) {

    owner = nullptr;
    if (collisionShape != nullptr) collisionShape->setLocation(this->location);
    gameWorld = nullptr;
    _isDead = false;
    this->angle = 0;
}

void GameEntity::render(double x, double y) {

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

Event *GameEntity::getCollisionEvent() {
    return this->collisionEvent;
}

void GameEntity::setWorld(GameWorld *gameWorld) {
    this->gameWorld = gameWorld;
}

GameWorld *GameEntity::getWorld() {
    return this->gameWorld;
}

GameEntity::~GameEntity() {
    delete collisionEvent;
}


void GameEntity::die() {
    _isDead = true;
}

bool GameEntity::isDead() {
    return _isDead;
}

bool GameEntity::collidesWith(GameEntity *otherEntity) {
    if (this->collisionShape == nullptr || otherEntity->collisionShape == nullptr) return false;
    if (this->collisionShape->intersectsWith(otherEntity->getCollisionShape())) return true;
    return otherEntity->getCollisionShape()->intersectsWith(this->collisionShape);
}

CollisionShape *GameEntity::getCollisionShape() {
    return this->collisionShape;
}

void GameEntity::setCollisionShape(CollisionShape* collisionShape) {
    this->collisionShape = collisionShape;
}

void GameEntity::setOwner(GameEntity* owner) {
    this->owner = owner;
}

GameEntity *GameEntity::getOwner() {
    return this->owner;
}
