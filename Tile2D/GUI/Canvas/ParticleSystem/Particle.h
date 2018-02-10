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


#ifndef SPACEGAME_PARTICLE_H
#define SPACEGAME_PARTICLE_H

#include "precompile.h"
#include "Body.h"
#include "Color.h"
class ParticleSystem;

class Particle {
    friend class ParticleSystem;

public:
    Particle();
    void destroy();

    // getters and setters
    Uint32 getTimeLived();
    Uint32 getSpawnTimestamp() const;
    float getOpacity() const;
    void setOpacity(float opacity);
    Transform &getTransform();
    const Color &getColor() const;
    void setColor(const Color &color);
    const Vecf &getVelocity() const;

    void setVelocity(const Vecf &velocity);

private:
    Uint32          spawnTimestamp_;
    float           opacity_;
    Vecf            velocity_;
    Transform       transform_;
    Color           color_;

    ParticleSystem* owner_;
    Particle*       next_;
    Particle*       prev_;
};


#endif //SPACEGAME_PARTICLE_H
