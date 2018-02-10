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

#include "precompile.h"
#include "Tile2DComponent.h"
#include "Particle.h"
#include "DrawableBase.h"
#include "Texture.h"

class ParticleSystem : public DrawableBase {
    friend class Particle;

public:
    ParticleSystem();

    // getters and setters
    GLenum          getBlendSourceFactor() const;
    void            setBlendSourceFactor(GLenum blendSourceFactor);
    GLenum          getBlendDestinationFactor() const;
    void            setBlendDestinationFactor(GLenum blendDestionationFactor);
    unsigned int    getSpawnFrequency() const;
    void            setSpawnFrequency(unsigned int spawnFrequency);
    unsigned int    getMaxParticles() const;
    void            setMaxParticles(unsigned int maxParticles);
    const           Rect &getParticleRect() const;
    void            setParticleRect(const Rect &particleRect);
    Texture         *getTexturePtr() const;
    void            setTexturePtr(Texture *texturePtr);
    void            setInitFunc(void (*initFunc)(Particle *));
    void            setUpdateFunc(void (*updateFunc)(Particle *));
    bool            playsOnce() const;
    void            setPlaysOnce(bool playOnce);

private:
    GLenum          blendSourceFactor_ = GL_SRC_ALPHA;
    GLenum          blendDestinationFactor_ = GL_ONE_MINUS_SRC_ALPHA;
    unsigned int    spawnFrequency_;
    unsigned int    maxParticles_;
    Rect            particleRect_;
    Texture*        texturePtr_ = nullptr;
    void            (*initFunc)(Particle*) = nullptr;
    void            (*updateFunc)(Particle*) = nullptr;
    bool            playsOnce_;

public:

protected:
    void onDestroy() override;

private:

    Particle*       firstParticle_ = nullptr;
    unsigned int    particleCount_ = 0;
    unsigned int    particlesSpawned_ = 0;

    Uint32          lastSpawnTimeStamp_;

    void drawActual(const Canvas &canvas) override;

    void spawnParticles_();
    void updateParticles_();
    void drawParticles_();
};

#endif //SPACEGAME_PARTICLESYSTEM_H
