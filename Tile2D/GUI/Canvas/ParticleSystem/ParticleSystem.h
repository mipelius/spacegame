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


#ifndef SPACEGAME_PARTICLESYSTEM_H
#define SPACEGAME_PARTICLESYSTEM_H

#include <SDL2/SDL_stdinc.h>
#include "Tile2DComponent.h"
#include "Particle.h"
#include "DrawableBase.h"

class ParticleSystem : DrawableBase {

public:
    ParticleSystem();

    // getters and setters
    unsigned int getSpawnFrequency() const;
    void setSpawnFrequency(unsigned int spawnFrequency);
    unsigned int getMaxParticles() const;
    void setMaxParticles(unsigned int maxParticles);

    void setInitFunc(void (*initFunc)(Particle *));
    void setUpdateFunc(void (*updateFunc)(Particle *));

private:
    unsigned int spawnFrequency_;
    unsigned int maxParticles_;
    unsigned int particleCount_;

    void (*initFunc)(Particle*) = nullptr;
    void (*updateFunc)(Particle*) = nullptr;

    Uint32 lastSpawnTimeStamp_;

    void drawActual(const Canvas &canvas) override;
};

#endif //SPACEGAME_PARTICLESYSTEM_H
