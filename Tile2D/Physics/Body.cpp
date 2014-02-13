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
#include "SimpleProperty.h"

Body::Body(double mass) :
    // properties

    mass            (   new SimpleProperty<double>  (&mass_             )   ),

    angle           (   new SimpleProperty<double>  (&angle_            )   ),
    angularVelocity (   new SimpleProperty<double>  (&angularVelocity_  )   ),
    torque          (   new SimpleProperty<double>  (&torque_           )   ),

    location        (   new SimpleProperty<Point>   (&location_         )   ),
    speed           (   new SimpleProperty<Vector>  (&speed_            )   ),
    velocity        (   new SimpleProperty<Vector>  (&velocity_         )   ),
    force           (   new SimpleProperty<Vector>  (&force_            )   ),

    // events

    bodyCollision   (   new Event(this) ),
    mapCollision    (   new Event(this) ),

    // private member objects

    location_       (   Point(0,0)  ),
    speed_          (   Vector(0,0) ),
    velocity_       (   Vector(0,0) ),
    force_          (   Vector(0,0) )

{
    mass_ = mass;

    angle_ = 0.0;
    angularVelocity_ = 0.0;
    torque_ = 0.0;

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
        // calculate air resistance

        Vector airResistance = Vector(0, 0);

        if (speed_.x != 0 || speed_.y != 0) {
            double speedLengthPow2 = speed_.x * speed_.x + speed_.y * speed_.y;
            Vector airResistanceUnitVector = (speed_ * -1) * (1 / sqrt(speed_.x * speed_.x + speed_.y * speed_.y));
            airResistance = airResistanceUnitVector * speedLengthPow2 * (0.5 * gameWorld_->airDensity->get());
        }

        Vector totalForce = force_ + airResistance;

        // use acceleration for updating speed --> velocity --> location

        Vector acceleration = totalForce * (1 / this->mass->get());
        acceleration += gameWorld_->gForce->get();
        speed_ += acceleration;
        velocity_ = speed_ * timeElapsedSec * gameWorld_->metersPerPixel->get();
        location_ = location_ + velocity_;

        // apply torque (not very nicely implemented, but good enough)

        angularVelocity_ = torque_ * timeElapsedSec;
        angle_ = angle_ + angularVelocity_;

        // remove all applied forces

        force_ = Vector(0, 0);
        torque_ = torque_ / (this->mass->get() * timeElapsedSec);

        // update dependent properties

        this->angle->updateDependentProperties();
        this->angularVelocity->updateDependentProperties();
        this->torque->updateDependentProperties();

        this->location->updateDependentProperties();
        this->velocity->updateDependentProperties();
        this->speed->updateDependentProperties();
        this->force->updateDependentProperties();
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