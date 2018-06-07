// MIT License
//
// This file is part of SpaceGame.
// Copyright (c) 2014-2018 Miika Pelkonen
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


#include "Tile2D.h"
#include "PhysicsWorld.h"
#include "GameObject.h"

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
    float velLength = sqrtf(velLengthSqr);

    if (velLength > 0.0f) {
        Vecf dragUnitVector = velocity_ / (-velLength);
        drag = dragUnitVector * velLengthSqr * (0.5 * world.getAirDensity()) * drag_;
    }

    Vecf totalForce = force_ + drag;

    // update position and velocity

    position += velocity_ * timeElapsedSec;

    Vecf acceleration = (totalForce / mass_) + (world.gForce_ * gravityFactor_);
    velocity_ += acceleration * timeElapsedSec;

    // apply rotation change

    rotation = rotation + angularVelocity_ * timeElapsedSec;

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

void Body::deserialize(const json::Object &jsonObject) {
    if (jsonObject.HasKey("mass")) {
        setMass(jsonObject["mass"].ToFloat());
    }
    if (jsonObject.HasKey("gravityFactor")) {
        setGravityFactor(jsonObject["gravityFactor"].ToFloat());
    }
    if (jsonObject.HasKey("drag")) {
        setDrag(jsonObject["drag"].ToFloat());
    }
}

Tile2DComponent *Body::clone() {
    return new Body(*this);
}
