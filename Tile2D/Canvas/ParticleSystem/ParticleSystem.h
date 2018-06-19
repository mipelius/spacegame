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



#ifndef SPACEGAME_PARTICLESYSTEM_H
#define SPACEGAME_PARTICLESYSTEM_H

#include "precompile.h"
#include "Tile2DComponent.h"
#include "Particle.h"
#include "DrawableBase.h"
#include "Texture.h"
#include "Rect.h"

class ParticleSystem :
        public DrawableBase
{
    friend class Particle;

public:
    class IUpdater {
    public:
        virtual void update(Particle* particle) = 0;
        virtual IUpdater* clone() = 0;
    };

    class IInitializer {
    public:
        virtual void initialize(Particle* particle) = 0;
        virtual IInitializer* clone() = 0;
    };

    class UpdaterWrapper : public IUpdater {
    private:
        void (*updateFunc_)(Particle*) = nullptr;
    public:
        explicit UpdaterWrapper(void (*updateFunctionPtr)(Particle *)) {
            updateFunc_ = updateFunctionPtr;
        }
        void update(Particle* particle) override {
            updateFunc_(particle);
        }
        IUpdater* clone() override {
            return new UpdaterWrapper(*this);
        }
    };

    class InitializerWrapper : public IInitializer {
    private:
        void (*initFunc_)(Particle*) = nullptr;
    public:
        explicit InitializerWrapper(void (*initFunctionPtr)(Particle *)) {
            initFunc_ = initFunctionPtr;
        }
        void initialize(Particle* particle) override {
            initFunc_(particle);
        };
        IInitializer* clone() override {
            return new InitializerWrapper(*this);
        };
    };

    ParticleSystem();
    ParticleSystem(ParticleSystem& other);
    ~ParticleSystem() override;

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
    void            setInitializer(void (*initFunc)(Particle *));
    void            setUpdater(void (*updateFunc)(Particle *));
    bool            playsOnce() const;
    void            setPlaysOnce(bool playOnce);

    void deserialize(const json::Object &jsonObject) override;

protected:
    void onDestroy() override;
    Tile2DComponent *clone() override;

private:
    GLenum          blendSourceFactor_ = GL_SRC_ALPHA;
    GLenum          blendDestinationFactor_ = GL_ONE_MINUS_SRC_ALPHA;
    unsigned int    spawnFrequency_;
    unsigned int    maxParticles_;
    Rect            particleRect_;
    Texture*        texturePtr_ = nullptr;
    bool            playsOnce_;
    Particle*       firstParticle_ = nullptr;
    unsigned int    particleCount_ = 0;
    unsigned int    particlesSpawned_ = 0;
    Uint32          lastSpawnTimeStamp_;

    IUpdater*       updater_ = nullptr;
    IInitializer*   initializer_ = nullptr;

    void drawActual(const Canvas &canvas) override;

    void spawnParticles_();
    void updateParticles_();
    void drawParticles_();

    std::map<std::string, GLenum> stringToGLEnumMap_ = {
            { "GL_SRC_ALPHA",           GL_SRC_ALPHA           },
            { "GL_ONE_MINUS_SRC_ALPHA", GL_ONE_MINUS_SRC_ALPHA },
            { "GL_ONE",                 GL_ONE                 },
    };
};

#endif //SPACEGAME_PARTICLESYSTEM_H
