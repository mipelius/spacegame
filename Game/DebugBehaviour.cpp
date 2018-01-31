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


#include <SDL2/SDL_events.h>
#include "DebugBehaviour.h"
#include "Spawner.h"
#include "Tile2D.h"
#include "Sprite.h"
#include "Body.h"
#include "WalkingEnemyAI.h"
#include "Health.h"
#include "Tile2DMath.h"
#include "PulseLightBehaviour.h"
#include "ParticleSystem.h"

void DebugBehaviour::awake() {
    body_ = gameObject()->getComponent<Body>();
    sprite_ = gameObject()->getComponent<Sprite>();
}

void DebugBehaviour::update() {
    float size = 1.0f;
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_1 : {
                        auto enemy = Spawner::spawnEnemy(
                                transform()->getPosition(),
                                "walking_alien_green",
                                {
                                        {-10 * size, -25 * size},
                                        {10 * size, -25 * size},
                                        {10 * size, 15 * size},
                                        {5 * size, 25 * size},
                                        {-5 * size, 25 * size},
                                        {-10 * size, 15 * size}
                                },
                                {-25 * size, -25 * size, 25 * size, 25 * size},
                                1.0f
                        );

                        auto AI = enemy->attachComponent<WalkingEnemyAI>();
                        AI->setTarget(transform());
                        AI->setGroundCheckPoints(
                                {
                                        {-10, 26.0f * size},
                                        {-5, 26.0f * size},
                                        {0, 26.0f * size},
                                        {5, 26.0f * size},
                                        {10, 26.0f * size}
                                }
                        );

                        auto health = enemy->attachComponent<Health>();
                        health->setMaxHealth(300);
                        health->onDeath.add([] (Health* health, GameObjectDiedEventArgs args) {
                            health->gameObject()->destroy();

                            auto explosion = Tile2D::createGameObject();
                            explosion->transform() = *(health->transform());

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
                                particle->setColor({1.0f, 0.0f, 0.0f});
                                particle->setOpacity((rand() % 200) / 400.0f + 0.5f);
                            });
                            explosionParticles->setUpdateFunc([] (Particle* particle){
                                if (particle->getTimeLived() > 1000) {
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
                            explosionParticles->setParticleRect({-32, -32, 32, 32});
                            explosionParticles->setTexturePtr(Tile2D::resources().textures["explosion_particle"]);
                            explosionParticles->setMaxParticles(100);
                            explosionParticles->setSpawnFrequency(300);
                            explosionParticles->setBlendSourceFactor(GL_SRC_ALPHA);
                            explosionParticles->setBlendDestinationFactor(GL_ONE);

                            explosion->transform().setScale({0.75, 0.75});
                        });

                        break;
                    }
                case SDLK_2 :
                    Spawner::spawnEnemy(
                            transform()->getPosition(),
                            "crab_kindof_colored",
                            {{-10 * size, -25 * size}, {10 * size, -25 * size}, {10 * size, 25 * size}, {-10 * size, 25 * size}},
                            {-25 * size, -25 * size, 25 * size, 25 * size},
                            0.0f
                    );
                    break;
                case SDLK_3:
                    Spawner::spawnEnemy(
                            transform()->getPosition(),
                            "fourwaycyclops_colored",
                            {{-30 * size, -24 * size}, {30 * size, -24 * size}, {30 * size, 24 * size}, {-30 * size, 24 * size}},
                            {-30 * size, -30 * size, 30 * size, 30 * size},
                            0.0f
                    );
                    break;
                case SDLK_4:
                    Spawner::spawnEnemy(
                            transform()->getPosition(),
                            "twohorn_colored",
                            {{-10 * size, -25 * size}, {10 * size, -25 * size}, {10 * size, 25 * size}, {-10 * size, 25 * size}},
                            {-25 * size, -25 * size, 25 * size, 25 * size},
                            0.0f
                    );
                    break;

                case SDLK_RETURN:
                    Tile2D::isDebugMode = !Tile2D::isDebugMode;
                    if (Tile2D::isDebugMode) {
                        sprite_->setOpacity(0.5);
                    } else {
                        sprite_->setOpacity(1.0);
                    }
                    break;
                case SDLK_q:
                    transform()->setPosition({500.0, 250.0});
                    break;
                case SDLK_r:
                    Tile2D::tileMap().load("maps/map.bmp", "json/tileset.json");
                    break;
                case SDLK_s:
                    Tile2D::lightSystem().setBlendedShadowsEnabled(!Tile2D::lightSystem().isBlendedShadowsEnabled());
                    break;
                case SDLK_d:
                    Tile2D::lightSystem().setSoftShadowsEnabled(!Tile2D::lightSystem().isSoftShadowsEnabled());
                default:
                    break;
            }
        }
    }
}

void DebugBehaviour::lateUpdate() {

}
