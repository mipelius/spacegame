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


#include "Resources.h"
#include "SceneInGame.h"
#include "SceneTitleScreen.h"
#include "SceneEndScreen.h"
#include "Scenes.h"
#include "SceneQuickTesting.h"
#include "ObjectCreator.h"
#include "GameObject.h"

// --- SERIALIZABLE CLASSES ---

// --- components

#include "Body.h"
#include "PolygonCollider.h"
#include "Sprite.h"
#include "AnimatedSprite.h"
#include "FlyingEnemyAI.h"
#include "EnemyTargetingComponent.h"
#include "Health.h"

// --- other

#include "Cannon.h"

// ... etc

#undef main

class DeathHandler :
        public IEventHandler<Health, GameObjectDiedEventArgs>,
        public ISerializable
{
public:
    void handle(Health* health, GameObjectDiedEventArgs args) const override {
        health->gameObject()->destroy();
    }

    void deserialize(const json::Object &jsonObject) override { }

    IEventHandler<Health, GameObjectDiedEventArgs>* clone() {
        return new DeathHandler(*this);
    };
};

int main(int argc, const char *argv[]) {
    Tile2D::load(
            "data/config.json",
            "data/resources.json",
            "data/gameSetup/sortingLayers.json",
            "data/gameSetup/colliderLayerMatrix.json",
            "data/gameSetup/tags.json",
            {
                    {Scenes::titleScreen,    new SceneTitleScreen},
                    {Scenes::inGame,         new SceneInGame},
                    {Scenes::gameEndScreen,  new SceneEndScreen},
                    {Scenes::quickTesting,   new SceneQuickTesting}
            },
            {
                    // components

                    { "AnimatedSprite",          new ObjectCreator<AnimatedSprite>()          },
                    { "Body",                    new ObjectCreator<Body>()                    },
                    { "PolygonCollider",         new ObjectCreator<PolygonCollider>()         },
                    { "Sprite",                  new ObjectCreator<Sprite>()                  },
                    { "FlyingEnemyAI",           new ObjectCreator<FlyingEnemyAI>()           },
                    { "EnemyTargetingComponent", new ObjectCreator<EnemyTargetingComponent>() },
                    { "Health",                  new ObjectCreator<Health>()                  },

                    // EventHandlers

                    { "DeathHandler",            new ObjectCreator<DeathHandler>()            },

                    // Items

                    { "Cannon",                  new ObjectCreator<Cannon>()                  },
            }
    );

    return 0;
}
