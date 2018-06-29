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

GameObject *Prefabs::player() {
    auto player = Tile2D::createGameObject();
    player->transform().setRotation(0.0f);

    player->setTag(Tile2D::getTag(0));

    auto debugBehaviour = player->attachComponent<DebugBehaviour>();

    auto spaceshipBody = player->attachComponent<Body>();
    spaceshipBody->setMass(100.0);

    auto polygonCollider = player->attachComponent<PolygonCollider>();
    polygonCollider->setPoints({
                                       {-20, -18},
                                       {-5, -18},
                                       {18, 0},
                                       {-5, 18},
                                       {-20, 18}
                               });
    polygonCollider->setLayer(Tile2D::physicsWorld().getColliderLayerMatrix().getColliderLayer(2));

    auto spaceshipSprite = player->attachComponent<Sprite>();
    spaceshipSprite->setRect({-20, -20, 20, 20});
    spaceshipSprite->setTexturePtr(Tile2D::resources().textures["spaceship"]);
    spaceshipSprite->setSortingLayer(Tile2D::canvas().getSortingLayer(5));

    auto light = player->attachComponent<PointLight>();
    light->setIntensity(1.0);
    light->setRadius(600.0);

    auto playerController = player->attachComponent<PlayerController>();
    playerController->moveForce = 100000.0f;

    auto health = player->attachComponent<Health>();
    health->setMaxHealth(300);
    health->setAutoHealingRate(30);
    health->onDeath.add([] (Health* health, GameObjectDiedEventArgs args) {
        GameObject* spaceShipExplosion = Tile2D::resources().prefabs["fx_explosion"]->instantiate();
        spaceShipExplosion->transform().setPosition(health->transform()->getPosition());
        health->gameObject()->setIsActive(false);

        Tile2D::executeDelayedFunction(health->gameObject(), 2000, [] (GameObject* gameObject) {
            gameObject->transform().setPosition({500.0f, 250.0f});
            gameObject->getComponent<Body>()->setVelocity({0.0f, 0.0f});
            gameObject->setIsActive(true);
            gameObject->getComponent<Health>()->reset();
        });
    });

    auto power = player->attachComponent<Power>();
    power->setMaxPower(300);
    power->setReloadSpeed(200);

    auto targetingComponent = player->attachComponent<PlayerTargetingComponent>();

    // --- ITEMS ---

    auto inventory = player->attachComponent<Inventory>();

    // -- 1 -- LASER
    auto laserCannon = inventory->attachItem<Cannon>(
            Tile2D::resources().textures["inventory_laser_cannon"],
            ItemTags::laser,
            true
    );
    laserCannon->setAmmoPrefab(Tile2D::resources().prefabs["ammo_laser"]);
    laserCannon->setOffsets({
                                    {-10, -13},
                                    {-10, 13}
                            });
    laserCannon->setPowerConsumption(50);
    laserCannon->setReloadDelay(100);
    laserCannon->setIsActivated(true);

    // -- 2 -- GATLING
    auto gatlingGun = inventory->attachItem<Cannon>(
            Tile2D::resources().textures["inventory_gatling"],
            ItemTags::gatling,
            true
    );
    gatlingGun->setAmmoPrefab(Tile2D::resources().prefabs["ammo_gatling"]);
    gatlingGun->setOffsets({
                                   {-10, -13},
                                   {0,   0},
                                   {-10, 13}
                           });
    gatlingGun->setPowerConsumption(20);
    gatlingGun->setReloadDelay(10);
    gatlingGun->setIsActivated(false);

    // -- 3 -- BOMBS
    auto bombDropper = inventory->attachItem<BombDropper>(
            Tile2D::resources().textures["inventory_bomb"],
            ItemTags::bombDropper,
            false
    );
    bombDropper->setPowerConsumption(200);
    bombDropper->setCount(0);
    bombDropper->setIsActivated(false);
    bombDropper->setBombPrefab_(Tile2D::resources().prefabs["ammo_bomb"]);

    // -- 4 -- PLASMA
    auto plasmaCannon = inventory->attachItem<Cannon>(
            Tile2D::resources().textures["inventory_plasma_cannon"],
            ItemTags::plasmaCannon,
            true
    );
    plasmaCannon->setAmmoPrefab(Tile2D::resources().prefabs["ammo_plasma"]);
    plasmaCannon->setPowerConsumption(300);
    plasmaCannon->setReloadDelay(500);
    plasmaCannon->setIsActivated(false);

    // -- 5 -- MEDIKIT
    auto medikit = inventory->attachItem<Medikit>(
            Tile2D::resources().textures["inventory_medikit"],
            ItemTags::medikit,
            false
    );
    medikit->setPowerConsumption(100);
    medikit->setHealingAmount(100);
    medikit->setIsActivated(false);
    medikit->setCount(0);

    return player;
}

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

GameObject *Prefabs::enemySpawner(
        Rect area,
        GameObject *target,
        const std::string& prefabString,
        Uint32 spawningDelay)
{
    GameObject* spawnerObj = Tile2D::createGameObject();
    auto spawnerBehaviour = spawnerObj->attachComponent<EnemySpawner>();
    spawnerBehaviour->setPlayer(target);
    spawnerBehaviour->setPrefab(prefabString);
    spawnerBehaviour->setSpawningDelay(spawningDelay);
    spawnerBehaviour->setAreaRect(area);
    spawnerBehaviour->setInnerRect({-600.0f, -400.0f, 600.0f, 400.0f});
    spawnerBehaviour->setOuterRect({-1000.0f, -800.0f, 1000.0f, 800.0f});

    return spawnerObj;
}
GameObject* Prefabs::pickupSpawner(
        Rect area,
        GameObject *target,
        const std::string& prefabString,
        Uint32 spawningDelay,
        int itemTag,
        int maxItemCount,
        float minDistanceToTarget
) {
    GameObject* spawnerObj = Tile2D::createGameObject();
    auto spawnerBehaviour = spawnerObj->attachComponent<PickupSpawner>();
    spawnerBehaviour->setPlayer(target);
    spawnerBehaviour->setPrefab(prefabString);
    spawnerBehaviour->setSpawningDelay(spawningDelay);
    spawnerBehaviour->setItemTag(itemTag);
    spawnerBehaviour->setMaxItemCount(maxItemCount);
    spawnerBehaviour->setMinDistanceToTarget(minDistanceToTarget);
    spawnerBehaviour->setAreaRect(area);

    return spawnerObj;
}

GameObject *Prefabs::hud(GameObject* player) {
    auto hud = Tile2D::createGameObject();
    auto hudBehaviour = hud->attachComponent<HUD>();
    hudBehaviour->setPlayer(player);
    return hud;
}