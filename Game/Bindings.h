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
#include "ObjectCreator.h"

#include "Body.h"
#include "PolygonCollider.h"
#include "Sprite.h"
#include "AnimatedSprite.h"
#include "FlyingEnemyAI.h"
#include "EnemyTargetingComponent.h"
#include "LimitedLifetimeBehaviour.h"
#include "Health.h"
#include "PointLight.h"
#include "Cannon.h"
#include "ParticleSystem.h"

#include "ParticleSystemComponents.h"
#include "EventHandlers.h"

class Bindings {
public:
    static std::map<std::string, IObjectCreator*> createBindings() {
        return {
            // components
            { "AnimatedSprite",                     new ObjectCreator<AnimatedSprite>()                 },
            { "Body",                               new ObjectCreator<Body>()                           },
            { "PolygonCollider",                    new ObjectCreator<PolygonCollider>()                },
            { "Sprite",                             new ObjectCreator<Sprite>()                         },
            { "FlyingEnemyAI",                      new ObjectCreator<FlyingEnemyAI>()                  },
            { "EnemyTargetingComponent",            new ObjectCreator<EnemyTargetingComponent>()        },
            { "Health",                             new ObjectCreator<Health>()                         },
            { "ParticleSystem",                     new ObjectCreator<ParticleSystem>()                 },
            { "LimitedLifetimeBehaviour",           new ObjectCreator<LimitedLifetimeBehaviour>()       },
            { "PointLight",                         new ObjectCreator<PointLight>()                     },

            // EventHandlers

            { "DeathHandler",                       new ObjectCreator<DeathHandler>()                   },
            { "AmmoCollisionDamageHandler",         new ObjectCreator<AmmoCollisionDamageHandler>()     },
            { "AmmoTerrainCollisionHandler",        new ObjectCreator<AmmoTerrainCollisionHandler>()    },

            // Particle settings

            { "ParticleInitializer",                new ObjectCreator<ParticleInitializer>()            },
            { "ParticleUpdater",                    new ObjectCreator<ParticleUpdater>()                },

            // Items

            { "Cannon",                             new ObjectCreator<Cannon>()                         }
        };
    }
};

#endif //SPACEGAME_BINDINGS_H