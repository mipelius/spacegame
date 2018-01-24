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


#include "BombBehaviour.h"
#include "Tile2D.h"
#include "Tile2DMath.h"
#include "ParticleSystem.h"
#include "PulseLightBehaviour.h"

void BombBehaviour::awake() {
    body_ = gameObject()->getComponent<Body>();
    body_->mapCollision.add([] (Body* body, MapCollisionEventArgs args) {
        for (auto x=-explosionRadius; x<explosionRadius; ++x) {
            for (auto y=-explosionRadius; y<explosionRadius; ++y) {
                if (Vecf((float)x, (float)y).length() > explosionRadius) {
                    continue;
                }
                Vecf offset = {
                        (float)(x * Tile2D::tileMap().getTileSet()->getTileW()),
                        (float)(y * Tile2D::tileMap().getTileSet()->getTileH())
                };
                Tile2D::tileMap().setValueScaled(
                        args.tileCoordinates + offset + args.velocityBeforeCollision.normalized() * 30.0f,
                        Tile2D::tileMap().getTileSet()->getEmptyBlock()
                );
            }
        }

        body->gameObject()->getComponent<Sprite>()->setIsVisible(false);
        body->gameObject()->destroy();

        auto explosion = Tile2D::createGameObject();
        explosion->transform() = *(body->transform());
        explosion->transform().setPosition(
                explosion->transform().getPosition() +
                args.velocityBeforeCollision.normalized() * 30.0f
        );

        auto explosionLight = explosion->attachComponent<PointLight>();
        explosionLight->setRadius(300.0f);
        explosionLight->setIntensity(1.0f);

        auto explosionPulseLightBehaviour = explosion->attachComponent<PulseLightBehaviour>();
        explosionPulseLightBehaviour->TTL = 2.0;
        explosionPulseLightBehaviour->setTimeToStartDiminish(1.0f);
        explosionPulseLightBehaviour->setRadiusDiminishSpeed(0.5f);
        explosionPulseLightBehaviour->setIntensityDiminishSpeed(1.5f);

        auto explosionParticles = explosion->attachComponent<ParticleSystem>();
        explosionParticles->setPlaysOnce(true);
        explosionParticles->setInitFunc([] (Particle* particle){
            Vecf pos = {(rand() % 10) * 5.0f - 25.0f, (rand() % 10) * 5.0f - 25.0f};
            pos *= 2;
            particle->getTransform().setPosition(pos);
            particle->getTransform().setRotation(rand() % 360);
            float size = 0.5f + (rand() % 255) / 255.0f;
            particle->getTransform().setScale({size, size});
            particle->setVelocity(pos.normalized() * (rand() % 2 + 3.0f));
            particle->setColor({1.0f, 1.0f, 1.0f});
            particle->setOpacity((rand() % 200) / 400.0f + 0.5f);
        });
        explosionParticles->setUpdateFunc([] (Particle* particle){
            if (particle->getTimeLived() > 2000) {
                particle->destroy();
            } else {
                Vecf pos = particle->getTransform().getPosition();
                particle->getTransform().setRotation(particle->getTransform().getRotation() + 1.0f);
                particle->getTransform().setPosition(pos + particle->getVelocity());
                particle->setOpacity(particle->getOpacity() - 0.01f);
                float newSize = particle->getTransform().getScale().x - 0.007f;
                Mathf::clamp(newSize, 0.0f, 100.0f);
                particle->getTransform().setScale({newSize, newSize});
            }
        });
        explosionParticles->setParticleRect({-64, -64, 64, 64});
        explosionParticles->setTexturePtr(Tile2D::resources().textures["explosion_particle"]);
        explosionParticles->setMaxParticles(100);
        explosionParticles->setSpawnFrequency(300);
        explosionParticles->setBlendSourceFactor(GL_SRC_ALPHA);
        explosionParticles->setBlendDestinationFactor(GL_ONE);

        explosion->transform().setScale({0.75, 0.75});
    });
}

void BombBehaviour::update() {

}

void BombBehaviour::lateUpdate() {
    transform()->setRotation(body_->getVelocity().angle());
}
