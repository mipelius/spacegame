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


#include <cfloat>
#include <Game/CollisionEffects.h>
#include "Medikit.h"
#include "ItemTags.h"
#include "Cannon.h"
#include "BombDropper.h"
#include "Inventory.h"
#include "AnimatedSprite.h"
#include "Scenes.h"
#include "Prefabs.h"
#include "PolygonCollider.h"
#include "WalkingEnemyAI.h"
#include "Tile2D.h"
#include "Sprite.h"
#include "PulseLightBehaviour.h"
#include "ParticleSystem.h"
#include "Tile2DMath.h"
#include "Health.h"
#include "BombBehaviour.h"
#include "SparkleBehaviour.h"
#include "FlyingEnemyAI.h"
#include "PlayerController.h"
#include "DebugBehaviour.h"
#include "BackgroundBehaviour.h"
#include "HUD.h"
#include "Resources.h"
#include "SceneManager.h"
#include "t2Time.h"
#include "Power.h"
#include "SwirlingBehaviour.h"
#include "PlayerTargetingComponent.h"
#include "EnemyTargetingComponent.h"
#include "AmmoComponent.h"
#include "PickupSpawner.h"
#include "Pickup.h"
#include "PhysicsWorld.h"

// ---- EFFECTS ----

GameObject *Prefabs::light() {
    auto light = Tile2D::createGameObject();

    auto lightBody = light->attachComponent<Body>();
    lightBody->setMass(10.0);
    lightBody->setVelocity(Vecf(0, 0));

    auto collider = light->attachComponent<PolygonCollider>();
    collider->setPoints({
                                {-5, -5},
                                {5,  -5},
                                {5,  5},
                                {-5, 5}
                        });

    auto lightSprite = light->attachComponent<Sprite>();
    lightSprite->setRect({-40, -40, 40, 40});
    lightSprite->setTexturePtr(Tile2D::resources().textures["fx_light"]);

    auto lightLight = light->attachComponent<PointLight>();
    lightLight->setRadius(100.0);
    lightLight->setIntensity(1.0);

    return light;
}

// --- OTHER ---


GameObject *Prefabs::hud(GameObject* player) {
    auto hud = Tile2D::createGameObject();
    auto hudBehaviour = hud->attachComponent<HUD>();
    hudBehaviour->setPlayer(player);
    return hud;
}