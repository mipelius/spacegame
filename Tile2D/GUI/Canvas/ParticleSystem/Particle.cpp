// This file is part of SpaceGame.
// Copyright (C) 2018 Miika Pelkonen
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
