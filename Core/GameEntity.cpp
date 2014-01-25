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
#include "GameEntity.h"

#include "CollisionShape.h"
#include "Event.h"
#include "GameWorld.h"
#include "EntityCollisionEventArgs.h"
#include "Map.h"
#include "MapCollisionEventArgs.h"

void GameEntity::applyForce(Vector force) {
    this->force += force;
}

void GameEntity::setAngle(double angle) {
    if (this->collisionShape != nullptr) this->collisionShape->setAngle(this->angle);
    this->angle = angle;
}

Point GameEntity::getLocation() {
    return this->location;
}

void GameEntity::setLocation(Point location) {
    if (this->collisionShape != nullptr) this->collisionShape->setLocation(location);
    this->location = location;
}

double GameEntity::getAngle() {
    return this->angle;
}

GameEntity::GameEntity(Point location, double angle, CollisionShape* collisionShape) :
    location(location),
    force(Vector(0, 0)),
    speed(Vector(0, 0)),
    velocity(Vector(0, 0)),
    entityCollisionEvent(new Event(this)),
    mapCollisionEvent(new Event(this)),
    collisionShape(collisionShape) {

    _entityCollisionDetectionIsIgnored = false;
    _stepIsIgnored  = false;

    _isDead = false;

    owner = nullptr;

    this->angle = angle;
    this->torque = 0;

    if (collisionShape != nullptr) collisionShape->setLocation(this->location);
    gameWorld = nullptr;

    if (collisionShape) collisionShape->setAngle(angle);
}

void GameEntity::render(double x, double y) {

}

double GameEntity::getMass() {
    return 0.001;
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

Event *GameEntity::getEntityCollisionEvent() {
    return this->entityCollisionEvent;
}

void GameEntity::_setWorld(GameWorld *gameWorld) {
    this->gameWorld = gameWorld;
}

GameWorld *GameEntity::getWorld() {
    return this->gameWorld;
}

GameEntity::~GameEntity() {
    delete entityCollisionEvent;
}


void GameEntity::die() {
    _isDead = true;
}

bool GameEntity::isDead() {
    return _isDead;
}

bool GameEntity::_detectCollisionWith(GameEntity *otherEntity) {
    beforeEntityCollisionDetection(otherEntity);

    if (!_entityCollisionDetectionIsIgnored) {
        if (this->collisionShape == nullptr || otherEntity->collisionShape == nullptr) return false;

        // TODO: this is ugly, make it better
        if (this->collisionShape->intersectsWith(otherEntity->getCollisionShape()) ||
            otherEntity->getCollisionShape()->intersectsWith(this->collisionShape)) {

            onEntityCollision(otherEntity);

            EntityCollisionEventArgs* args = new EntityCollisionEventArgs(otherEntity);
            entityCollisionEvent->raise(args);
            delete args;

            return true;
        }
    }

    _entityCollisionDetectionIsIgnored = false;

    return false;
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

Vector GameEntity::getVelocity() {
    return velocity;
}

void GameEntity::setVelocity(Vector velocity) {
    this->velocity = velocity;
}

Point GameEntity::getLocationBeforeUpdate() {
    return location - velocity;
}

void GameEntity::applyTorque(double angle) {
    this->torque += angle;
}

double GameEntity::getAngularVelocity() {
    return angularVelocity;
}

void GameEntity::setAngularVelocity(double angularVelocity) {
    this->angularVelocity = angularVelocity;
}

double GameEntity::getTorque() {
    return this->torque;
}

void GameEntity::setTorque(double torque) {
    this->torque = torque;
}

void GameEntity::step(double timeElapsedSec) {
    this->beforeStep(timeElapsedSec);

    if (!_stepIsIgnored) {
        // calculate air resistance

        Vector airResistance = Vector(0, 0);

        if (speed.x != 0 || speed.y != 0) {
            double speedLengthPow2 = speed.x * speed.x + speed.y * speed.y;
            Vector airResistanceUnitVector = (speed * -1) * (1 / sqrt(speed.x * speed.x + speed.y * speed.y));
            airResistance = airResistanceUnitVector * speedLengthPow2 * (0.5 * gameWorld->getAirDensity());
        }

        Vector totalForce = force + airResistance;

        // use acceleration for updating speed --> velocity --> location
        Vector acceleration = totalForce * (1 / this->getMass());
        acceleration += Vector(gameWorld->getGForce().x, gameWorld->getGForce().y);
        speed += acceleration;
        velocity = speed * timeElapsedSec * gameWorld->getMetersPerPixel();
        this->setLocation(location + velocity);

        // apply torque (not very nicely implemented, but good enough)
        angularVelocity = torque * timeElapsedSec;
        this->setAngle(angle + angularVelocity);

        // remove all applied forces
        this->setForceToZero();
        this->setTorque(torque / (this->getMass() * timeElapsedSec));
    }

    this->afterStep(timeElapsedSec);

    _stepIsIgnored = false;
}


void GameEntity::ignoreStep() {
    this->_stepIsIgnored = true;
}

void GameEntity::ignoreEntityCollisionDetection() {
    this->_entityCollisionDetectionIsIgnored = true;
}

bool GameEntity::_detectMapCollision() {
    if (this->getWorld()->getMap()->detectCollisionWith(this)) {

        this->onMapCollision();

        MapCollisionEventArgs* args = new MapCollisionEventArgs(this->getWorld()->getMap());
        mapCollisionEvent->raise(args);
        delete args;

        return true;
    }
    return false;
}

Event *GameEntity::getMapCollisionEvent() {
    return mapCollisionEvent;
}

// protected virtual methods

void GameEntity::afterStep(double timeElapsedSec) { }
void GameEntity::beforeStep(double timeElapsedSec) { }

void GameEntity::beforeEntityCollisionDetection(GameEntity *otherEntity) { }

void GameEntity::onEntityCollision(GameEntity *otherEntity) { }
void GameEntity::onMapCollision() { }

