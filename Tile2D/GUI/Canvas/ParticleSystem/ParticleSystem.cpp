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


#include <SDL2/SDL_timer.h>
#include "ParticleSystem.h"
#include "Tile2D.h"

ParticleSystem::ParticleSystem() {
    lastSpawnTimeStamp_ = SDL_GetTicks();
}


unsigned int ParticleSystem::getSpawnFrequency() const {
    return spawnFrequency_;
}

void ParticleSystem::setSpawnFrequency(unsigned int spawnFrequency) {
    spawnFrequency_ = spawnFrequency;
}

unsigned int ParticleSystem::getMaxParticles() const {
    return maxParticles_;
}

void ParticleSystem::setMaxParticles(unsigned int maxParticles) {
    maxParticles_ = maxParticles;
}

void ParticleSystem::drawActual(const Canvas &canvas) {
    // spawn new particles if necessary and possible
    if (spawnFrequency_ == 0) {
        return;
    }
    if (initFunc != nullptr) {
        Uint32 deltaTime = SDL_GetTicks() - lastSpawnTimeStamp_;
        float threshold = 1000.0f / spawnFrequency_;
        if (deltaTime > threshold) {
            Uint32 amount = (Uint32)(deltaTime / threshold);
            for (auto i = 0u; i < amount; ++i) {
                Particle* particle = Tile2D::instance_().particlePool_.malloc();
                initFunc(particle);
            }
        }
    }

}

void ParticleSystem::setInitFunc(void (*initFunc)(Particle *)) {
    ParticleSystem::initFunc = initFunc;
}

void ParticleSystem::setUpdateFunc(void (*updateFunc)(Particle *)) {
    ParticleSystem::updateFunc = updateFunc;
}
