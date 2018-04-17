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



#include "precompile.h"
#include "ParticleSystem.h"
#include "Tile2D.h"

ParticleSystem::ParticleSystem() : particleRect_({-10, -10, 10, 10}){
    lastSpawnTimeStamp_ = SDL_GetTicks();
}

void ParticleSystem::drawActual(const Canvas &canvas) {
    spawnParticles_();
    updateParticles_();
    drawParticles_();
}

void ParticleSystem::spawnParticles_() {
    // spawn new particles if necessary and possible
    if (spawnFrequency_ == 0) {
        return;
    }
    if (playsOnce_ && particlesSpawned_ >= maxParticles_) {
        return;
    }
    if (initFunc != nullptr) {
        Uint32 deltaTime = SDL_GetTicks() - lastSpawnTimeStamp_;
        float threshold = 1000.0f / spawnFrequency_;
        if (deltaTime > threshold) {
            lastSpawnTimeStamp_ = SDL_GetTicks();
            Uint32 amount = (Uint32)(deltaTime / threshold);
            for (auto i = 0u; i < amount; ++i) {
                if (particleCount_ > maxParticles_) {
                    break;
                }
                Particle* newParticle = Tile2D::instance_().particlePool_.malloc();

                if (firstParticle_ != nullptr) {
                    firstParticle_->prev_ = newParticle;
                }
                newParticle->next_ = firstParticle_;
                newParticle->prev_ = nullptr;
                newParticle->owner_ = this;
                firstParticle_ = newParticle;

                newParticle->spawnTimestamp_ = lastSpawnTimeStamp_;
                initFunc(newParticle);
                particleCount_++;
                particlesSpawned_++;
            }
        }
    }
}

void ParticleSystem::updateParticles_() {
    Particle* currentParticle = firstParticle_;
    while (currentParticle != nullptr) {
        updateFunc(currentParticle);
        currentParticle = currentParticle->next_;
    }
}

void ParticleSystem::drawParticles_() {
    if (texturePtr_ == nullptr) {
        return;
    }
    texturePtr_->glBind();
    Particle* currentParticle = firstParticle_;

    glBlendFunc(blendSourceFactor_, blendDestinationFactor_);

    while (currentParticle != nullptr) {
        glMatrixMode(GL_MODELVIEW);
        currentParticle->transform_.glTransform();

        glColor4f(
                currentParticle->color_.red,
                currentParticle->color_.green,
                currentParticle->color_.blue,
                currentParticle->opacity_
        );

        glBegin(GL_QUADS);

        texturePtr_->glTexCorner(Texture::Corner::TOP_LEFT);
        glVertex3f(particleRect_.x1, particleRect_.y1, 0.0);

        texturePtr_->glTexCorner(Texture::Corner::TOP_RIGHT);
        glVertex3f(particleRect_.x2, particleRect_.y1, 0.0);

        texturePtr_->glTexCorner(Texture::Corner::BOTTOM_RIGHT);
        glVertex3f(particleRect_.x2, particleRect_.y2, 0.0);

        texturePtr_->glTexCorner(Texture::Corner::BOTTOM_LEFT);
        glVertex3f(particleRect_.x1, particleRect_.y2, 0.0);

        glEnd();

        currentParticle->transform_.glInvTransform();

        currentParticle = currentParticle->next_;
    }
}

// --- getters and setters ---

Texture *ParticleSystem::getTexturePtr() const {
    return texturePtr_;
}

void ParticleSystem::setTexturePtr(Texture *texturePtr) {
    texturePtr_ = texturePtr;
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

void ParticleSystem::setInitFunc(void (*initFunc)(Particle *)) {
    ParticleSystem::initFunc = initFunc;
}

void ParticleSystem::setUpdateFunc(void (*updateFunc)(Particle *)) {
    ParticleSystem::updateFunc = updateFunc;
}

const Rect &ParticleSystem::getParticleRect() const {
    return particleRect_;
}

void ParticleSystem::setParticleRect(const Rect &particleRect) {
    particleRect_ = particleRect;
}

void ParticleSystem::onDestroy() {
    DrawableBase::onDestroy();
    Particle* currentParticle = firstParticle_;

    while (currentParticle != nullptr) {
        Particle* nextParticle = currentParticle->next_;
        Tile2D::instance_().particlePool_.free(currentParticle);
        currentParticle = nextParticle;
    }
}

GLenum ParticleSystem::getBlendSourceFactor() const {
    return blendSourceFactor_;
}

void ParticleSystem::setBlendSourceFactor(GLenum blendSourceFactor) {
    blendSourceFactor_ = blendSourceFactor;
}

GLenum ParticleSystem::getBlendDestinationFactor() const {
    return blendDestinationFactor_;
}

void ParticleSystem::setBlendDestinationFactor(GLenum blendDestionationFactor) {
    blendDestinationFactor_ = blendDestionationFactor;
}

bool ParticleSystem::playsOnce() const {
    return playsOnce_;
}

void ParticleSystem::setPlaysOnce(bool playOnce) {
    playsOnce_ = playOnce;
}
