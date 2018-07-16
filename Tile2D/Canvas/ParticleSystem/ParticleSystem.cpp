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
#include "Resources.h"
#include "Reflector.h"

ParticleSystem::ParticleSystem() : particleRect_({-10, -10, 10, 10}){
    lastSpawnTimeStamp_ = SDL_GetTicks();
}

void ParticleSystem::drawActual(const Canvas &canvas) {
    if (isPlaying_) {
        spawnParticles_();
    }
    updateParticles_();
    drawParticles_();
}

void ParticleSystem::spawnParticles_() {
    // spawn new particles if necessary and possible
    if (spawnFrequency_ == 0) {
        return;
    }
    if (initializer_ != nullptr) {
        Uint32 deltaTime = SDL_GetTicks() - lastSpawnTimeStamp_;
        float threshold = 1000.0f / spawnFrequency_;
        if (deltaTime > threshold) {
            Uint32 amount = (Uint32)(deltaTime / threshold);
            for (auto i = 0u; i < amount; ++i) {
                if (playsOnce_ && particlesSpawned_ >= particlesToSpawn_) {
                    stop();
                    return;
                }
                if (!emitOnce()) {
                    return;
                }
            }
        }
    }
}

void ParticleSystem::updateParticles_() {
    Particle* currentParticle = firstParticle_;
    while (currentParticle != nullptr) {
        updater_->update(currentParticle);
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

        glTexCoord2f(0.01, 0.01);
        glVertex3f(particleRect_.x1, particleRect_.y1, 0.0);

        glTexCoord2f(0.99, 0.01);
        glVertex3f(particleRect_.x2, particleRect_.y1, 0.0);

        glTexCoord2f(0.99, 0.99);
        glVertex3f(particleRect_.x2, particleRect_.y2, 0.0);

        glTexCoord2f(0.01, 0.99);
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

void ParticleSystem::setInitializer(void (*initFunc)(Particle *)) {
    initializer_ = new InitializerWrapper(initFunc);
}

void ParticleSystem::setUpdater(void (*updateFunc)(Particle *)) {
    updater_ = new UpdaterWrapper(updateFunc);
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

Tile2DComponent *ParticleSystem::clone() {
    return new ParticleSystem(*this);
}

ParticleSystem::~ParticleSystem() {
    delete initializer_;
    delete updater_;
}

void ParticleSystem::deserialize(const json::Object &jsonObject) {
    DrawableBase::deserialize(jsonObject);

    if (jsonObject.HasKey("blendSourceFactor")) {
        auto str = jsonObject["blendSourceFactor"].ToString();

        auto it = stringToGLEnumMap_.find(str);
        if (it == stringToGLEnumMap_.end()) {
            throw std::runtime_error("ParticleSystem: invalid blendSourceFactor");
        }
        blendSourceFactor_ = (*it).second;
    }
    if (jsonObject.HasKey("blendDestinationFactor")) {
        auto str = jsonObject["blendDestinationFactor"].ToString();

        auto it = stringToGLEnumMap_.find(str);
        if (it == stringToGLEnumMap_.end()) {
            throw std::runtime_error("ParticleSystem: invalid blendDestinationFactor_");
        }
        blendDestinationFactor_ = (*it).second;
    }
    if (jsonObject.HasKey("spawnFrequency")) {
        spawnFrequency_ = (unsigned int)jsonObject["spawnFrequency"].ToInt();
    }
    if (jsonObject.HasKey("maxParticles")) {
        maxParticles_ = (unsigned int)jsonObject["maxParticles"].ToInt();
    }
    if (jsonObject.HasKey("particleRect")) {
        particleRect_.deserialize(jsonObject["particleRect"].ToObject());
    }
    if (jsonObject.HasKey("texture")) {
        auto textureName = jsonObject["texture"].ToString();
        texturePtr_ = Tile2D::resources().textures[textureName];
    }
    if (jsonObject.HasKey("playsOnce")) {
        playsOnce_ = jsonObject["playsOnce"].ToBool();
    }
    if (jsonObject.HasKey("initializer")) {
        auto initializerJson = jsonObject["initializer"];
        auto initializer = Tile2D::reflector().instantiate<IInitializer>(initializerJson);
        initializer_ = initializer;
    }
    if (jsonObject.HasKey("updater")) {
        auto updaterJson = jsonObject["updater"];
        auto updater = Tile2D::reflector().instantiate<IUpdater>(updaterJson);
        updater_ = updater;
    }
    if (jsonObject.HasKey("playOnAwake")) {
        playOnAwake_ = jsonObject["playOnAwake"].ToBool();
    }
    if (jsonObject.HasKey("particlesToSpawn")) {
        particlesToSpawn_ = (unsigned int)jsonObject["particlesToSpawn"].ToInt();
    }
}

ParticleSystem::ParticleSystem(ParticleSystem &other) {
    blendSourceFactor_ = other.blendSourceFactor_;
    blendDestinationFactor_ = other.blendDestinationFactor_;
    spawnFrequency_ = other.spawnFrequency_;
    maxParticles_ = other.maxParticles_;
    particleRect_ = other.particleRect_;
    texturePtr_ = other.texturePtr_;
    playsOnce_ = other.playsOnce_;
    playOnAwake_ = other.playOnAwake_;
    particlesToSpawn_ = other.particlesToSpawn_;

    firstParticle_ = nullptr;
    particleCount_ = 0;
    particlesSpawned_ = 0;
    lastSpawnTimeStamp_ = 0;

    if (other.initializer_ != nullptr) {
        this->initializer_ = other.initializer_->clone();
    }
    if (other.updater_ != nullptr) {
        this->updater_ = other.updater_->clone();
    }
}

void ParticleSystem::play() {
    isPlaying_ = true;
}

void ParticleSystem::stop() {
    particlesSpawned_ = 0;
    isPlaying_ = false;
}

void ParticleSystem::init() {
    DrawableBase::init();

    if (playOnAwake_) {
        play();
    }
}

bool ParticleSystem::isPlayOnAwake() const {
    return playOnAwake_;
}

void ParticleSystem::setPlayOnAwake(bool playOnAwake) {
    playOnAwake_ = playOnAwake;
}

bool ParticleSystem::isPlaying() const {
    return isPlaying_;
}

void ParticleSystem::setInitializer(ParticleSystem::IInitializer* initializer) {
    initializer_ = initializer;
}

void ParticleSystem::setUpdater(ParticleSystem::IUpdater* updater) {
    updater_ = updater;
}

unsigned int ParticleSystem::getParticlesToSpawn() const {
    return particlesToSpawn_;
}

void ParticleSystem::setParticlesToSpawn(unsigned int particlesToSpawn) {
    particlesToSpawn_ = particlesToSpawn;
}

bool ParticleSystem::emitOnce() {
    if (particleCount_ > maxParticles_) {
        return false;
    }

    Particle* newParticle = Tile2D::instance_().particlePool_.malloc();

    if (firstParticle_ != nullptr) {
        firstParticle_->prev_ = newParticle;
    }
    newParticle->next_ = firstParticle_;
    newParticle->prev_ = nullptr;
    newParticle->owner_ = this;
    firstParticle_ = newParticle;

    lastSpawnTimeStamp_ = SDL_GetTicks();
    newParticle->spawnTimestamp_ = lastSpawnTimeStamp_;
    initializer_->initialize(newParticle);
    particleCount_++;
    particlesSpawned_++;

    return true;
}
