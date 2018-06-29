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

#ifndef SPACEGAME_PARTICLESYSTEMCOMPONENTS_H
#define SPACEGAME_PARTICLESYSTEMCOMPONENTS_H

#include "ParticleSystem.h"
#include "PointLight.h"
#include "Tile2DMath.h"
#include "t2Time.h"

class ParticleInitializer :
        public ParticleSystem::IInitializer,
        public ISerializable
{
private:
    Color color_ = {1.0f, 1.0f, 1.0f};
    float velocityCoefficient_ = 80.0f;

public:
    void initialize(Particle *particle) override {
        Vecf pos = {(rand() % 10) * 5.0f - 25.0f, (rand() % 10) * 5.0f - 25.0f};
        pos = pos.normalized();
        particle->getTransform().setPosition(pos);
        particle->getTransform().setRotation(rand() % 360);
        float size = 0.5f + (rand() % 255) / 255.0f;
        particle->getTransform().setScale({size, size});
        particle->setVelocity(pos * ((rand() % 2 + 3.0f) * velocityCoefficient_));
        particle->setColor(color_);
        particle->setOpacity((rand() % 200) / 400.0f + 0.5f);
    }

    IInitializer *clone() override {
        return new ParticleInitializer(*this);
    }

    void deserialize(const json::Object &jsonObject) override {
        if (jsonObject.HasKey("color")) {
            auto colorJson = jsonObject["color"].ToObject();
            color_.deserialize(colorJson);
        }
        if (jsonObject.HasKey("velocityCoefficient")) {
            velocityCoefficient_ = jsonObject["velocityCoefficient"].ToFloat();
        }
    }
};

class ParticleUpdater :
        public ParticleSystem::IUpdater,
        public ISerializable
{
private:
    Uint32 timeToLive_ = 2000;

public:
    void update(Particle *particle) override {
        if (particle->getTimeLived() > timeToLive_) {
            particle->destroy();
        } else {
            auto deltaTime = Tile2D::time().getDeltaTime();

            Vecf pos = particle->getTransform().getPosition();
            particle->getTransform().setRotation(particle->getTransform().getRotation() + 1.0f * deltaTime);
            particle->getTransform().setPosition(pos + (particle->getVelocity() * deltaTime));
            particle->setOpacity(particle->getOpacity() - (deltaTime / (timeToLive_ / 1000.0f)));
            float newSize = particle->getTransform().getScale().x - 0.007f * deltaTime;
            Mathf::clamp(newSize, 0.0f, 100.0f);
            particle->getTransform().setScale({newSize, newSize});
        }
    }

    IUpdater *clone() override {
        return new ParticleUpdater(*this);
    }

    void deserialize(const json::Object &jsonObject) override {
        if (jsonObject.HasKey("timeToLive")) {
            timeToLive_ = (Uint32)jsonObject["timeToLive"].ToInt();
        }
    }
};

#endif //SPACEGAME_PARTICLESYSTEMCOMPONENTS_H
