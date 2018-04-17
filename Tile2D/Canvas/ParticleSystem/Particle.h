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
