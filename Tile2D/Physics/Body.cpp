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
#include "Body.h"

#include "CollisionShape.h"
#include "Event.h"
#include "PhysicsWorld.h"
#include "Map.h"
#include "BodyCollisionEventArgs.h"

Body::Body(double mass) :
    mass(               new Property<double>    (mass)          ),

    angle(              new Property<double>    (0.0)           ),
    angularVelocity(    new Property<double>    (0.0)           ),
    torque(             new Property<double>    (0.0)           ),

    location(           new Property<Point>     (Point(0, 0))   ),
    speed(              new Property<Vector>    (Vector(0, 0))  ),
    velocity(           new Property<Vector>    (Vector(0, 0))  ),
    force(              new Property<Vector>    (Vector(0, 0))  ),

    bodyCollision(      new Event(this)                         ),
    mapCollision(       new Event(this)                         )

{
    entityCollisionDetectionIsIgnored_ = false;
    stepIsIgnored_ = false;
    gameWorld_ = nullptr;
    collisionShape_ = nullptr;
}

Body::~Body() {
    delete mass;

    delete angle;
    delete angularVelocity;
    delete torque;

    delete location;
    delete speed;
    delete velocity;
    delete force;

    delete bodyCollision;
    delete mapCollision;
}

void Body::step_(double timeElapsedSec) {
    if (!stepIsIgnored_) {

        // store values from properties to local variables
        // (we don't want to set the property values during the calculations)

        double angle = this->angle->get();
        double angularVelocity = this->angularVelocity->get();
        double torque = this->torque->get();

        Point location = this->location->get();
        Vector velocity = this->velocity->get();
        Vector speed = this->speed->get();
        Vector force = this->force->get();

        // calculate air resistance

        Vector airResistance = Vector(0, 0);

        if (speed.x != 0 || speed.y != 0) {
            double speedLengthPow2 = speed.x * speed.x + speed.y * speed.y;
            Vector airResistanceUnitVector = (speed * -1) * (1 / sqrt(speed.x * speed.x + speed.y * speed.y));
            airResistance = airResistanceUnitVector * speedLengthPow2 * (0.5 * gameWorld_->airDensity->get());
        }

        Vector totalForce = force + airResistance;

        // use acceleration for updating speed --> velocity --> location

        Vector acceleration = totalForce * (1 / this->mass->get());
        acceleration += gameWorld_->gForce->get();
        speed += acceleration;
        velocity = speed * timeElapsedSec * gameWorld_->metersPerPixel->get();
        location = location + velocity;

        // apply torque (not very nicely implemented, but good enough)

        angularVelocity = torque * timeElapsedSec;
        angle = angle + angularVelocity;

        // remove all applied forces

        force = Vector(0, 0);
        torque = torque / (this->mass->get() * timeElapsedSec);

        // store values to properties

        this->angle->set(angle);
        this->angularVelocity->set(angularVelocity);
        this->torque->set(torque);

        this->location->set(location);
        this->velocity->set(velocity);
        this->speed->set(speed);
        this->force->set(force);
    }

    stepIsIgnored_ = false;
}


bool Body::detectMapCollision_() {
    Map* map = this->getWorld()->getMap();
    if (map && map->detectCollisionWith(this)) {

        EventArgs* args = new EventArgs;
        mapCollision->raise(args);
        delete args;

        return true;
    }
    return false;
}


bool Body::detectCollisionWith_(Body *otherBody) {
    if (!entityCollisionDetectionIsIgnored_) {
        if (this->collisionShape_ == nullptr || otherBody->collisionShape_ == nullptr) return false;

        // TODO: this is ugly, make it better
        if (this->collisionShape_->intersectsWith(otherBody->getCollisionShape()) ||
                otherBody->getCollisionShape()->intersectsWith(this->collisionShape_)) {

            BodyCollisionEventArgs* args = new BodyCollisionEventArgs();
            args->otherBody = otherBody;
            bodyCollision->raise(args);
            delete args;

            return true;
        }
    }

    entityCollisionDetectionIsIgnored_ = false;

    return false;
}

void Body::ignoreStep() {
    this->stepIsIgnored_ = true;
}

void Body::ignoreCollisionDetection() {
    this->entityCollisionDetectionIsIgnored_ = true;
}

void Body::applyForce(Vector force) {
    Vector forceBefore = this->force->get();
    this->force->set(forceBefore + force);
}

void Body::setWorld_(PhysicsWorld *gameWorld) {
    this->gameWorld_ = gameWorld;
}

PhysicsWorld *Body::getWorld() {
    return this->gameWorld_;
}

CollisionShape *Body::getCollisionShape() {
    return this->collisionShape_;
}

void Body::setCollisionShape(CollisionShape* collisionShape) {
    this->collisionShape_ = collisionShape;
    collisionShape->owner = this;
}

void Body::applyTorque(double angle) {
    double torqueBefore = this->torque->get();
    this->torque->set(torqueBefore + angle);
}