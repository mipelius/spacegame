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

#include "Tile2D.h"

Body::Body() :
    velocity_       (   Vecf(0,0) ),
    force_          (   Vecf(0,0) ),
    drag_           (   1.0f      ),
    angularVelocity_(   0.0f      ),
    mass_           (   1.0f      ),
    gravityFactor_  (   1.0f      )
{

}

void Body::init() {
    Tile2D::physicsWorld().bodies_.push_back(this);
}

void Body::onDestroy() {
    Tile2D::physicsWorld().bodies_.remove(this);
}

void Body::step_(float timeElapsedSec) {
    PhysicsWorld& world = Tile2D::physicsWorld();

    Vecf& position = gameObject()->transform().position_;
    float& rotation = gameObject()->transform().rotation_;

    // calculate drag

    Vecf drag = {0.0f, 0.0f};

    float velLengthSqr = velocity_.lengthSqr();
    float velLength = sqrt(velLengthSqr);

    if (velLength > 0.0f) {
        Vecf dragUnitVector = (velocity_ * -1) / velLength;
        drag = dragUnitVector * velLengthSqr * (0.5 * world.getAirDensity()) * drag_;
    }

    Vecf totalForce = force_ + drag;

    // use acceleration for updating velocity --> position

    Vecf acceleration = totalForce / mass_;
    acceleration += world.gForce_ * gravityFactor_;
    velocity_ += acceleration;
    position = position + (velocity_ * timeElapsedSec * world.getMetersPerPixel());

    // apply rotation change

    rotation = rotation + angularVelocity_;

    // remove all applied forces

    force_ = Vecf(0.0, 0.0);
}

void Body::applyForce(Vecf force) {
    force_ += force;
}

// getters and setters

float Body::getMass() const {
    return mass_;
}

void Body::setMass(float mass) {
    mass_ = mass;
}

float Body::getAngularVelocity() const {
    return angularVelocity_;
}

void Body::setAngularVelocity(float angularVelocity) {
    angularVelocity_ = angularVelocity;
}

const Vecf &Body::getVelocity() const {
    return velocity_;
}

void Body::setVelocity(const Vecf &velocity) {
    velocity_ = velocity;
}

float Body::getDrag() const {
    return drag_;
}

void Body::setDrag(float drag) {
    drag_ = drag;
}

float Body::getGravityFactor() const {
    return gravityFactor_;
}

void Body::setGravityFactor(float gravityFactor) {
    gravityFactor_ = gravityFactor;
}
