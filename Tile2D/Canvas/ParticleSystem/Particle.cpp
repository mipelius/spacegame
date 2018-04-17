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



#include "Particle.h"
#include "ParticleSystem.h"
#include "Tile2D.h"

Particle::Particle() : color_({0, 0, 0}), transform_(Transform()) {

}

void Particle::destroy() {
    if (prev_ != nullptr) {
        prev_->next_ = next_;
    }
    else {  // (this == owner_->firstParticle_) and we need to update it
        owner_->firstParticle_ = next_;
    }
    if (next_ != nullptr) {
        next_->prev_ = prev_;
    }

    owner_->particleCount_--;
    Particle* particle = this;
    Tile2D::instance_().particlePool_.free(particle);
}

// getters and setters

Uint32 Particle::getSpawnTimestamp() const {
    return spawnTimestamp_;
}

float Particle::getOpacity() const {
    return opacity_;
}

void Particle::setOpacity(float opacity) {
    opacity_ = opacity;
}

Transform &Particle::getTransform() {
    return transform_;
}

const Color &Particle::getColor() const {
    return color_;
}

void Particle::setColor(const Color &color) {
    color_ = color;
}

Uint32 Particle::getTimeLived() {
    return SDL_GetTicks() - spawnTimestamp_;
}

const Vecf &Particle::getVelocity() const {
    return velocity_;
}

void Particle::setVelocity(const Vecf &velocity) {
    velocity_ = velocity;
}
