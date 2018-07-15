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

#ifndef SPACEGAME_BINDINGS_H
#define SPACEGAME_BINDINGS_H

#include <map>
#include "AudioSource.h"

#include "SceneInGame.h"
#include "SceneTitleScreen.h"
#include "SceneVictory.h"

#include "Medikit.h"
#include "PlayerTargetingComponent.h"
#include "PlayerController.h"
#include "PickupSpawner.h"
#include "PlasmaExplosionBehaviour.h"
#include "ExplosionBehaviour.h"
#include "BombBehaviour.h"
#include "BombDropper.h"

#include "Background.h"
#include "BackgroundBehaviour.h"
#include "Pickup.h"
#include "ObjectCreator.h"

#include "Body.h"
#include "PolygonCollider.h"
#include "Sprite.h"
#include "AnimatedSprite.h"
#include "SwirlingBehaviour.h"
#include "FlyingEnemyAI.h"
#include "EnemyTargetingComponent.h"
#include "LimitedLifetimeBehaviour.h"
#include "Health.h"
#include "PointLight.h"
#include "Cannon.h"
#include "ParticleSystem.h"
#include "WalkingEnemyAI.h"
#include "PopUpBehaviour.h"
#include "ParticleSystemComponents.h"
#include "EventHandlers.h"

#include "Inventory.h"

class Bindings {
public:
    static std::map<std::string, IObjectCreator*> createBindings() {
        return {
                // Scenes

                {"SceneTitleScreen",                    new ObjectCreator<SceneTitleScreen>()},
                {"SceneInGame",                         new ObjectCreator<SceneInGame>()},
                {"SceneVictory",                        new ObjectCreator<SceneVictory>()},

                // components

                {"AnimatedSprite",                      new ObjectCreator<AnimatedSprite>()},
                {"Background",                          new ObjectCreator<Background>()},
                {"BackgroundBehaviour",                 new ObjectCreator<BackgroundBehaviour>()},
                {"Body",                                new ObjectCreator<Body>()},
                {"BombDropper",                         new ObjectCreator<BombDropper>()},
                {"BombBehaviour",                       new ObjectCreator<BombBehaviour>()},
                {"PolygonCollider",                     new ObjectCreator<PolygonCollider>()},
                {"Sprite",                              new ObjectCreator<Sprite>()},
                {"FlyingEnemyAI",                       new ObjectCreator<FlyingEnemyAI>()},
                {"EnemyTargetingComponent",             new ObjectCreator<EnemyTargetingComponent>()},
                {"Health",                              new ObjectCreator<Health>()},
                {"ParticleSystem",                      new ObjectCreator<ParticleSystem>()},
                {"LimitedLifetimeBehaviour",            new ObjectCreator<LimitedLifetimeBehaviour>()},
                {"SwirlingBehaviour",                   new ObjectCreator<SwirlingBehaviour>()},
                {"PointLight",                          new ObjectCreator<PointLight>()},
                {"WalkingEnemyAI",                      new ObjectCreator<WalkingEnemyAI>()},
                {"Pickup",                              new ObjectCreator<Pickup>()},
                {"PulseLightBehaviour",                 new ObjectCreator<PulseLightBehaviour>()},
                {"ExplosionBehaviour",                  new ObjectCreator<ExplosionBehaviour>()},
                {"PlasmaExplosionBehaviour",            new ObjectCreator<PlasmaExplosionBehaviour>()},
                {"EnemySpawner",                        new ObjectCreator<EnemySpawner>()},
                {"PickupSpawner",                       new ObjectCreator<PickupSpawner>()},
                {"PlayerController",                    new ObjectCreator<PlayerController>()},
                {"Power",                               new ObjectCreator<Power>()},
                {"PlayerTargetingComponent",            new ObjectCreator<PlayerTargetingComponent>()},
                {"Inventory",                           new ObjectCreator<Inventory>()},
                {"AudioSource",                         new ObjectCreator<AudioSource>()},
                {"PopUpBehaviour",                      new ObjectCreator<PopUpBehaviour>()},

                // EventHandlers

                {"EnemyDeathHandler",                   new ObjectCreator<EnemyDeathHandler>()},
                {"PlayerDeathHandler",                  new ObjectCreator<PlayerDeathHandler>()},
                {"AmmoCollisionDamageHandler",          new ObjectCreator<AmmoCollisionDamageHandler>()},
                {"AmmoTerrainCollisionHandler",         new ObjectCreator<AmmoTerrainCollisionHandler>()},
                {"ContinuousCollisionDamageHandler",    new ObjectCreator<ContinuousCollisionDamageHandler>()},
                {"PowerUpgradeCollisionHandler",        new ObjectCreator<PowerUpgradeCollisionHandler>()},
                {"HealthUpgradeCollisionHandler",       new ObjectCreator<HealthUpgradeCollisionHandler>()},
                {"ItemPickupCollisionEventHandler",     new ObjectCreator<ItemPickupCollisionEventHandler>()},
                {"CannonUpgradeCollisionEventHandler",  new ObjectCreator<CannonUpgradeCollisionEventHandler>()},
                {"CollisionAudioHandler",               new ObjectCreator<CollisionAudioHandler>()},
                {"TerrainCollisionAudioHandler",        new ObjectCreator<TerrainCollisionAudioHandler>()},

                // Particle settings

                {"ParticleInitializer",                 new ObjectCreator<ParticleInitializer>()},
                {"ParticleUpdater",                     new ObjectCreator<ParticleUpdater>()},

                // Items

                {"Cannon",                              new ObjectCreator<Cannon>()},
                {"Medikit",                             new ObjectCreator<Medikit>()}
        };
    }
};

#endif //SPACEGAME_BINDINGS_H