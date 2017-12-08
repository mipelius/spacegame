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

#include <Tile2D/Core/Tile2D.h>
#include "precompile.h"
#include "Body.h"

#include "CollisionShape.h"
#include "PhysicsWorld.h"
#include "WorldMap.h"
#include "BodyCollisionEventArgs.h"

Body::Body() :
    // properties

    mass            (   Property<double>  (&mass_             )   ),

    angle           (   Property<double>  (&angle_            )   ),
    angularVelocity (   Property<double>  (&angularVelocity_  )   ),
    torque          (   Property<double>  (&torque_           )   ),

    position        (   Property<Vec>  (&position_         )   ),
    speed           (   Property<Vec>  (&speed_            )   ),
    velocity        (   Property<Vec>  (&velocity_         )   ),
    force           (   Property<Vec>  (&force_            )   ),

    // events

    bodyCollision   (   Event<Body, BodyCollisionEventArgs>(this) ),
    mapCollision    (   Event<Body, EventArgs>(this) ),

    // private member variables

    position_       (   Vec(0,0) ),
    speed_          (   Vec(0,0) ),
    velocity_       (   Vec(0,0) ),
    force_          (   Vec(0,0) ),
    angle_          (   0.0         ),
    angularVelocity_(   0.0         ),
    torque_         (   0.0         ),
    mass_           (   1.0         ),

    entityCollisionDetectionIsIgnored_  (false),
    stepIsIgnored_                      (false),
    physicsWorld_                       (nullptr),
    collisionShape_                     (nullptr),
    isDead_                             (false)
{

}

void Body::init() {
    Tile2D::physicsWorld().add(this);
}

void Body::onDestroy() {
    Tile2D::physicsWorld().remove(this);
}

void Body::step_(double timeElapsedSec) {
    if (!stepIsIgnored_) {
        // calculate air resistance

        Vec airResistance = Vec(0, 0);

        if (speed_.x != 0 || speed_.y != 0) {
            double speedLengthPow2 = speed_.x * speed_.x + speed_.y * speed_.y;
            Vec airResistanceUnitVector = (speed_ * -1) * (1 / sqrt(speed_.x * speed_.x + speed_.y * speed_.y));
            airResistance = airResistanceUnitVector * speedLengthPow2 * (0.5 * physicsWorld_->airDensity.get());
        }

        Vec totalForce = force_ + airResistance;

        // use acceleration for updating speed --> velocity --> location

        Vec acceleration = totalForce * (1 / this->mass.get());
        acceleration += physicsWorld_->gForce.get();
        speed_ += acceleration;
        velocity_ = speed_ * timeElapsedSec * physicsWorld_->metersPerPixel.get();
        position_ = position_ + velocity_;

        // apply torque (not very nicely implemented, but good enough)

        angularVelocity_ = torque_ * timeElapsedSec;
        angle_ = angle_ + angularVelocity_;

        // remove all applied forces

        force_ = Vec(0, 0);
        torque_ = torque_ / (this->mass.get() * timeElapsedSec);

        // update dependent properties

        this->angle.updateDependentProperties();
        this->angularVelocity.updateDependentProperties();
        this->torque.updateDependentProperties();

        this->position.updateDependentProperties();
        this->velocity.updateDependentProperties();
        this->speed.updateDependentProperties();
        this->force.updateDependentProperties();
    }

    stepIsIgnored_ = false;
}

bool Body::detectMapCollision_() {
    WorldMap* map = this->physicsWorld_->map_;
    if (map == nullptr) {
        return false;
    }

    if (map->detectCollisionWith(*this)) {

        mapCollision.raise(EventArgs());

        return true;
    }
    return false;
}


bool Body::detectCollisionWith_(Body &otherBody) {
    if (!entityCollisionDetectionIsIgnored_) {
        if (this->collisionShape_ == nullptr || otherBody.collisionShape_ == nullptr) return false;
        if (this->collisionShape_->intersectsWith(otherBody.getCollisionShape()) ||
                otherBody.getCollisionShape().intersectsWith(*collisionShape_)) {

            bodyCollision.raise(BodyCollisionEventArgs(otherBody));

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

void Body::applyForce(Vec force) {
    Vec forceBefore = this->force.get();
    this->force.set(forceBefore + force);
}

void Body::setWorld_(PhysicsWorld *gameWorld) {
    this->physicsWorld_ = gameWorld;
}

PhysicsWorld *Body::getWorld() {
    return this->physicsWorld_;
}

CollisionShape& Body::getCollisionShape() {
    return *this->collisionShape_;
}

void Body::setCollisionShape(CollisionShape* collisionShape) {
    this->collisionShape_ = collisionShape;
    collisionShape->owner_ = this;
}

void Body::applyTorque(double angle) {
    double torqueBefore = this->torque.get();
    this->torque.set(torqueBefore + angle);
}
