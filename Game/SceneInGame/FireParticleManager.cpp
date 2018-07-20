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

#include "Tile2DMath.h"
#include "FireParticleManager.h"
#include "Tile2D.h"
#include "GameObject.h"
#include "Vec.h"
#include "Resources.h"
#include "t2Time.h"

FireParticleManager* FireParticleManager::instance_ = nullptr;

FireParticleManager *FireParticleManager::getInstance() {
    if (instance_ == nullptr) {
        // NOTE: Hack for ASM2018 game dev competition, can't adjust properties in json :(
        auto gameObject = Tile2D::createGameObject();

        instance_ = gameObject->attachComponent<FireParticleManager>();

        auto particleSystem = gameObject->attachComponent<ParticleSystem>();
        particleSystem->transform()->setPosition({0.0f, 0.0f});
        particleSystem->setPlaysOnce(true);
        particleSystem->setPlayOnAwake(false);
        particleSystem->setMaxParticles(1000);
        particleSystem->setParticlesToSpawn(100);
        particleSystem->setParticleRect({-8, -8, 8, 8});
        particleSystem->setTexturePtr(Tile2D::resources().textures["fx_explosion_particle"]);
        particleSystem->setSpawnFrequency(1);
        particleSystem->setSortingLayer(Tile2D::canvas().getSortingLayer(4));
        particleSystem->setBlendSourceFactor(GL_SRC_ALPHA);
        particleSystem->setBlendDestinationFactor(GL_ONE);

        instance_->initializer_ = new ParticleSystemInitializer();
        instance_->updater_ = new ParticleSystemUpdater();
        particleSystem->setInitializer(instance_->initializer_);
        particleSystem->setUpdater(instance_->updater_);

        instance_->particleSystem_ = particleSystem;
    }

    return instance_;
}

void FireParticleManager::init() { }

void FireParticleManager::onDestroy() {
    instance_ = nullptr;
}

Tile2DComponent *FireParticleManager::clone() {
    return nullptr;
}

void FireParticleManager::createParticle(const Vecf &position, const Vecf &velocity) {
    initializer_->position = position;
    initializer_->velocity = velocity;
    particleSystem_->emitOnce();
}

void FireParticleManager::ParticleSystemInitializer::initialize(Particle *particle) {
    auto& transform = particle->getTransform();
    transform.setPosition(position);
    auto randomScale = Mathf::random(-0.4f, 0.4f);
    transform.setScale({1.0f + randomScale, 1.0f + randomScale});
    auto randomRotation = Mathf::random(0.0f, 360.f);
    transform.setRotation(randomRotation);

    particle->setVelocity(velocity);
    particle->setOpacity(1.0f);
    particle->setColor({1.0f, 1.0f, 1.0f});

}

ParticleSystem::IInitializer *FireParticleManager::ParticleSystemInitializer::clone() {
    return new ParticleSystemInitializer(*this);
}

void FireParticleManager::ParticleSystemUpdater::update(Particle *particle) {
    auto opacity = particle->getOpacity() - Tile2D::time().getDeltaTime() * 6.6f;
    particle->setOpacity(opacity);

    auto position = particle->getTransform().getPosition() + particle->getVelocity() * Tile2D::time().getDeltaTime();
    particle->getTransform().setPosition(position);

    if (SDL_GetTicks() - particle->getSpawnTimestamp() > 150) {
        particle->destroy();
    }
}

ParticleSystem::IUpdater *FireParticleManager::ParticleSystemUpdater::clone() {
    return new ParticleSystemUpdater(*this);
}
