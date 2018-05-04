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
#include <Game/SceneInGame/Weapons/Laser.h>
#include <Game/SceneInGame/Weapons/BombDropper.h>
#include <Game/SceneInGame/Weapons/Weapons.h>
#include "WeaponSystem.h"
#include "AnimatedSprite.h"
#include "Scenes.h"
#include "Prefabs.h"
#include "PolygonCollider.h"
#include "WalkingEnemyAI.h"
#include "Tile2D.h"
#include "Tags.h"
#include "Sprite.h"
#include "PulseLightBehaviour.h"
#include "ParticleSystem.h"
#include "Tile2DMath.h"
#include "Health.h"
#include "BombBehaviour.h"
#include "SparkleBehaviour.h"
#include "ColliderLayers.h"
#include "FlyingEnemyAI.h"
#include "SortingLayers.h"
#include "PlayerController.h"
#include "DebugBehaviour.h"
#include "BackgroundBehaviour.h"
#include "HUD.h"
#include "Resources.h"
#include "SceneManager.h"
#include "t2Time.h"
#include "Power.h"

GameObject *Prefabs::player() {
    auto player = Tile2D::createGameObject();
    player->transform().setRotation(0.0f);

    player->tag = Tags::player;

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
    polygonCollider->setLayer(ColliderLayers::player);

    auto spaceshipSprite = player->attachComponent<Sprite>();
    spaceshipSprite->setSortingLayer(SortingLayers::player);
    spaceshipSprite->setRect({-20, -20, 20, 20});
    spaceshipSprite->setTexturePtr(Tile2D::resources().textures["spaceship"]);

    auto light = player->attachComponent<PointLight>();
    light->setIntensity(1.0);
    light->setRadius(500.0);

    auto playerController = player->attachComponent<PlayerController>();
    playerController->moveForce = 100000.0f;

    auto health = player->attachComponent<Health>();
    health->setMaxHealth(600);
    health->onDeath.add([] (Health* health, GameObjectDiedEventArgs args) {
        GameObject* spaceShipExplosion = explosion();
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
    power->setMaxPower(600);
    power->setReloadSpeed(200);

    // --- WEAPONS ---

    auto weaponSystem = player->attachComponent<WeaponSystem>();

    auto laser = weaponSystem->attachWeapon<Laser>(
            Tile2D::resources().textures["laser_cannon"],
            nullptr,
            Weapons::laser
    );
    laser->setPowerConsumption(50);
    laser->setReloadDelay(100);
    laser->setIsActivated(true);

    auto bombDropper = weaponSystem->attachWeapon<BombDropper>(
            Tile2D::resources().textures["bomb"],
            nullptr,
            Weapons::bomb
    );
    bombDropper->setPowerConsumption(200);
    bombDropper->setReloadDelay(200);
    bombDropper->setIsActivated(true);

    auto placeholderWeapon1 = weaponSystem->attachWeapon<Laser>(
            Tile2D::resources().textures["laser_cannon"],
            nullptr,
            Weapons::laser
    );
    placeholderWeapon1->setPowerConsumption(50);
    placeholderWeapon1->setReloadDelay(100);
    placeholderWeapon1->setIsActivated(false);

    auto placeholderWeapon2 = weaponSystem->attachWeapon<Laser>(
            Tile2D::resources().textures["laser_cannon"],
            nullptr,
            Weapons::laser
    );
    placeholderWeapon2->setPowerConsumption(10);
    placeholderWeapon2->setReloadDelay(20);
    placeholderWeapon2->setIsActivated(true);

    return player;
}

// ---- ENEMIES ----

GameObject *Prefabs::boss() {
    auto enemy = spawnEnemy_(
            "evil_eye",
            {{-50, -50}, {50, -50}, {50, 50}, {-50, 50}},
            {-80, -80, 80, 80},
            0.0f
    );
    auto health = enemy->getComponent<Health>();
    health->setMaxHealth(1000);
    health->onDeath.add([] (Health* health, GameObjectDiedEventArgs args) {
        Tile2D::executeDelayedFunction(nullptr, 2000, [] (GameObject* gameObject) {
            Tile2D::sceneManager().loadScene(Scenes::gameEndScreen);
        });
    });

    auto AI = enemy->attachComponent<FlyingEnemyAI>();
    AI->setMaxDistance(100000);
    AI->setMaxNodesPathFinderExplores(2000);
    AI->setSpeed(150.0f);
    AI->setRotates(false);

    CountDownTimer shootingTimer;
    shootingTimer.setInterval(200);
    AI->setShootingTimer(shootingTimer);
    AI->setMaxPathFindingDistance(1500);

    return enemy;
}

GameObject *Prefabs::walker() {
    auto enemy = Tile2D::createGameObject();
    enemy->transform().setRotation(0.0f);

    enemy->tag = Tags::enemy;

    auto enemyBody = enemy->attachComponent<Body>();
    enemyBody->setMass(100.0);
    enemyBody->setGravityFactor(2.0f);

    auto polygonCollider = enemy->attachComponent<PolygonCollider>();
    polygonCollider->setPoints({
           {-18, -12},
           {18, -12},
           {18, 11},
           {11, 22},
           {-11, 22},
           {-18, 11}
    });
    polygonCollider->setLayer(ColliderLayers::enemy);
    polygonCollider->setSweepingStrategyThreshold(FLT_MAX);

    auto enemyAnim = enemy->attachComponent<AnimatedSprite>();
    enemyAnim->setSortingLayer(SortingLayers::enemyBackground);
    enemyAnim->setRect({-24, -18, 24, 30});
    enemyAnim->setAnimationPtr(Tile2D::resources().animations["walker"]);
    enemyAnim->play();

    auto health = enemy->attachComponent<Health>();
    health->setMaxHealth(100);
    health->onDeath.add([] (Health* health, GameObjectDiedEventArgs args) {
        health->gameObject()->destroy();
        GameObject* newBloodBurst = bloodBurst();
        newBloodBurst->transform().setPosition(health->transform()->getPosition());
    });

    auto AI = enemy->attachComponent<WalkingEnemyAI>();
    AI->setMaxDistance(1500);
    AI->setGroundCheckSensors(
            {
                    {-16, 23.0f},
                    {-8, 23.0f},
                    {0, 23.0f},
                    {8, 23.0f},
                    {16, 23.0f}
            }
    );

    return enemy;
}


GameObject *Prefabs::fish() {
    auto enemy = Tile2D::createGameObject();
    enemy->transform().setRotation(0.0f);

    enemy->tag = Tags::enemy;

    auto enemyBody = enemy->attachComponent<Body>();
    enemyBody->setMass(100.0);
    enemyBody->setGravityFactor(0.0f);

    auto polygonCollider = enemy->attachComponent<PolygonCollider>();
    polygonCollider->setPoints({{-42, -15}, {42, -15}, {42, 15}, {-42, 15}});
    polygonCollider->setLayer(ColliderLayers::enemy);
    polygonCollider->collision.add([] (PolygonCollider* collider, CollisionEventArgs args) {
        if (args.otherCollider->gameObject()->tag == Tags::player) {
            auto playerHealth = args.otherCollider->gameObject()->getComponent<Health>();
            playerHealth->damage(Tile2D::time().getDeltaTimeMS() / 2, collider->gameObject());
            sparkles(args.otherCollider->transform()->getPosition(), args.contactNormal, {0.0f, 1.0f, 0.0f});
        }
    });

    auto enemySprite = enemy->attachComponent<AnimatedSprite>();
    enemySprite->setSortingLayer(SortingLayers::enemy);
    enemySprite->setRect({-42, -42, 42, 42});
    enemySprite->setAnimationPtr(Tile2D::resources().animations["fish"]);
    enemySprite->setFramesPerSecond(50);
    enemySprite->play();

    auto health = enemy->attachComponent<Health>();
    health->setMaxHealth(100);
    health->onDeath.add([] (Health* health, GameObjectDiedEventArgs args) {
        health->gameObject()->destroy();
        GameObject* newBloodBurst = bloodBurst();
        newBloodBurst->transform().setPosition(health->transform()->getPosition());
    });

    auto AI = enemy->attachComponent<FlyingEnemyAI>();

    CountDownTimer pathUpdateTimer;
    pathUpdateTimer.setInterval(200);
    AI->setPathUpdateTimer(pathUpdateTimer);

    AI->setMaxPathFindingDistance(3000);
    AI->setMinPathFindingDistance(0);
    AI->setMaxNodesPathFinderExplores(2000);

    AI->setSpeed(300);

    CountDownTimer shootingTimer;
    shootingTimer.setInterval(5000);
    shootingTimer.setIntervalRandomness(2000);
    AI->setShootingTimer(shootingTimer);

    AI->setMaxDistance(1500);
    return enemy;
}

GameObject *Prefabs::evilEye() {
    auto enemy = spawnEnemy_(
            "evil_eye",
            {{-20, -20}, {20, -20}, {20, 20}, {-20, 20}},
            {-30, -30, 30, 30},
            0.0f
    );
    auto AI = enemy->attachComponent<FlyingEnemyAI>();
    CountDownTimer pathUpdateTimer;
    pathUpdateTimer.setInterval(1000);
    AI->setPathUpdateTimer(pathUpdateTimer);

    AI->setMaxPathFindingDistance(1500);

    CountDownTimer shootingTimer;
    shootingTimer.setInterval(500);
    shootingTimer.setIntervalRandomness(200);
    AI->setShootingTimer(shootingTimer);

    AI->setSpeed(300);
    AI->setRotates(false);

    AI->setMaxDistance(1500);

    return enemy;
}

GameObject *Prefabs::twoHorn() {
    auto enemy = spawnEnemy_(
            "twohorn",
            {{-10, -25}, {10, -25}, {10, 25}, {-10, 25}},
            {-25, -25, 25, 25},
            0.0f
    );

    auto AI = enemy->attachComponent<FlyingEnemyAI>();
    AI->setMaxDistance(1500);

    return enemy;
}

GameObject *Prefabs::spawnEnemy_(
        std::string textureName,
        std::vector<Vecf> colliderPoints,
        Rect spriteRect,
        float gravityFactor
) {
    auto enemy = Tile2D::createGameObject();
    enemy->transform().setRotation(0.0f);

    enemy->tag = Tags::enemy;

    auto enemyBody = enemy->attachComponent<Body>();
    enemyBody->setMass(100.0);
    enemyBody->setGravityFactor(gravityFactor);

    auto polygonCollider = enemy->attachComponent<PolygonCollider>();
    polygonCollider->setPoints(colliderPoints);
    polygonCollider->setLayer(ColliderLayers::enemy);

    auto enemySprite = enemy->attachComponent<Sprite>();
    enemySprite->setSortingLayer(SortingLayers::enemy);
    enemySprite->setRect(spriteRect);
    enemySprite->setTexturePtr(Tile2D::resources().textures[textureName]);
    enemySprite->setColor({1, 1, 1});

    auto health = enemy->attachComponent<Health>();
    health->setMaxHealth(100);
    health->onDeath.add([] (Health* health, GameObjectDiedEventArgs args) {
        health->gameObject()->destroy();
        GameObject* newBloodBurst = bloodBurst();
        newBloodBurst->transform().setPosition(health->transform()->getPosition());
    });

    return enemy;
}

// ---- PLAYER STUFF TO DROP / SHOOT ----


GameObject *Prefabs::bomb() {
    static const int explosionRadius = 10; // tiles

    auto bomb = Tile2D::createGameObject();

    auto body = bomb->attachComponent<Body>();
    body->setMass(50);

    auto collider = bomb->attachComponent<PolygonCollider>();
    collider->setPoints({{-9, -4}, {9, -4}, {9, 4}, {-9, 4}});
    collider->terrainCollision.add([] (PolygonCollider* collider, TerrainCollisionEventArgs args) {
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

        collider->gameObject()->getComponent<Sprite>()->setIsVisible(false);
        collider->gameObject()->destroy();

        auto explosion = Prefabs::explosion();

        explosion->transform() = *(collider->transform());
        explosion->transform().setPosition(
                explosion->transform().getPosition() +
                        args.velocityBeforeCollision.normalized() * 30.0f
        );

        explosion->transform().setScale({0.75, 0.75});
    });

    auto sprite = bomb->attachComponent<Sprite>();
    sprite->setSortingLayer(SortingLayers::ammo);
    sprite->setTexturePtr(Tile2D::resources().textures["bomb"]);
    sprite->setRect({-10, -10, 10, 10});

    auto bombBehaviour = bomb->attachComponent<BombBehaviour>();

    return bomb;
}

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
    lightSprite->setSortingLayer(SortingLayers::ammo);
    lightSprite->setRect({-40, -40, 40, 40});
    lightSprite->setTexturePtr(Tile2D::resources().textures["light"]);

    auto lightLight = light->attachComponent<PointLight>();
    lightLight->setRadius(100.0);
    lightLight->setIntensity(1.0);

    return light;
}

GameObject *Prefabs::laser() {
    return ammo(Tile2D::resources().textures["laser"]);
}

GameObject *Prefabs::enemyLaser() {
    return ammo(Tile2D::resources().textures["ammo_evil_eye"]);
}

GameObject *Prefabs::ammo(Texture* texturePtr) {
    auto laser = Tile2D::createGameObject();

    auto laserBody = laser->attachComponent<Body>();
    laserBody->setMass(10.0f);
    laserBody->setDrag(0.0f);

    auto laserSprite = laser->attachComponent<Sprite>();
    laserSprite->setRect({-20,-5,20,5});
    laserSprite->setTexturePtr(texturePtr);
    laserSprite->setSortingLayer(SortingLayers::ammo);

    auto laserLifetime = laser->attachComponent<LimitedLifetimeBehaviour>();
    laserLifetime->getTimer().setInterval(1000);

    auto laserLight = laser->attachComponent<PointLight>();
    laserLight->setRadius(80.0);
    laserLight->setIntensity(1.0);

    auto collider = laser->attachComponent<PolygonCollider>();
    collider->setPoints({
            {-18, -5},
            {18,  -5},
            {18,  5},
            {-18, 5}
    });
    collider->terrainCollision.add([] (PolygonCollider* collider, TerrainCollisionEventArgs args) {
        collider->gameObject()->destroy();
        Tile2D::tileMap().setValueScaled(args.tileCoordinates, Tile2D::tileMap().getTileSet()->getEmptyBlock());

        sparkles(args.tileCoordinates, args.contactNormal, {1, 1, 1});
        pulseLight(collider->transform()->getPosition());
    });
    collider->collision.add([] (PolygonCollider* collider, CollisionEventArgs args) {
        auto otherBody = args.otherCollider->gameObject()->getComponent<Body>();
        auto laserBody = collider->gameObject()->getComponent<Body>();

        if (args.otherCollider->gameObject()->tag == Tags::enemy) {
            otherBody->setVelocity(otherBody->getVelocity() + laserBody->getVelocity() / 100.0);
            sparkles(collider->transform()->getPosition(), args.contactNormal, {1, 0, 0});
            laserBody->gameObject()->destroy();

            auto health = args.otherCollider->gameObject()->getComponent<Health>();

            if (health != nullptr) {
                health->damage(10, laserBody->gameObject());
            }

            pulseLight(collider->transform()->getPosition());
        }
        if (args.otherCollider->gameObject()->tag == Tags::player) {
            otherBody->setVelocity(otherBody->getVelocity() + laserBody->getVelocity() / 100.0);
            sparkles(collider->transform()->getPosition(), args.contactNormal, {0, 1, 0});
            laserBody->gameObject()->destroy();

            auto health = args.otherCollider->gameObject()->getComponent<Health>();

            if (health != nullptr) {
                health->damage(10, laserBody->gameObject());
            }

            pulseLight(collider->transform()->getPosition());
        }
    });
    return laser;
}

// ---- EFFECTS ----

GameObject* Prefabs::bloodBurst() {
    auto bloodBurst = Tile2D::createGameObject();

    auto light = bloodBurst->attachComponent<PointLight>();
    light->setRadius(300.0f);
    light->setIntensity(1.0f);

    auto pulseLightBehaviour = bloodBurst->attachComponent<PulseLightBehaviour>();
    pulseLightBehaviour->getTimer().setInterval(2000);
    pulseLightBehaviour->setTimeToStartDiminish(1000);
    pulseLightBehaviour->setRadiusDiminishSpeed(0.5f);
    pulseLightBehaviour->setIntensityDiminishSpeed(1.5f);

    auto particles = bloodBurst->attachComponent<ParticleSystem>();
    particles->setSortingLayer(SortingLayers::particles);
    particles->setPlaysOnce(true);
    particles->setInitFunc([] (Particle* particle){
        Vecf pos = {(rand() % 10) * 5.0f - 25.0f, (rand() % 10) * 5.0f - 25.0f};
        pos *= 2;
        particle->getTransform().setPosition(pos);
        particle->getTransform().setRotation(rand() % 360);
        float size = 0.25f + (rand() % 255) / 255.0f;
        particle->getTransform().setScale({size, size});
        particle->setVelocity(pos.normalized() * (rand() % 2 + 3.0f));
        particle->setColor({1.0f, 0.0f, 0.0f});
        particle->setOpacity((rand() % 200) / 400.0f + 0.5f);
    });
    particles->setUpdateFunc([] (Particle* particle){
        if (particle->getTimeLived() > 2000) {
            particle->destroy();
        } else {
            auto deltaTime = Tile2D::time().getDeltaTime() * 60.0f;

            Vecf pos = particle->getTransform().getPosition();
            particle->getTransform().setRotation(particle->getTransform().getRotation() + 1.0f * deltaTime);
            particle->getTransform().setPosition(pos + particle->getVelocity() * deltaTime);
            particle->setOpacity(particle->getOpacity() - 0.01f * deltaTime);
            float newSize = particle->getTransform().getScale().x - 0.007f * deltaTime;
            Mathf::clamp(newSize, 0.0f, 100.0f);
            particle->getTransform().setScale({newSize, newSize});
        }
    });
    particles->setParticleRect({-32, -32, 32, 32});
    particles->setTexturePtr(Tile2D::resources().textures["explosion_particle"]);
    particles->setMaxParticles(100);
    particles->setSpawnFrequency(300);
    particles->setBlendSourceFactor(GL_SRC_ALPHA);
    particles->setBlendDestinationFactor(GL_ONE);

    bloodBurst->transform().setScale({0.5, 0.5});

    return bloodBurst;
}

GameObject *Prefabs::explosion() {
    auto explosion = Tile2D::createGameObject();

    auto explosionLight = explosion->attachComponent<PointLight>();
    explosionLight->setRadius(300.0f);
    explosionLight->setIntensity(1.0f);

    auto explosionPulseLightBehaviour = explosion->attachComponent<PulseLightBehaviour>();
    explosionPulseLightBehaviour->getTimer().setInterval(2000);
    explosionPulseLightBehaviour->setTimeToStartDiminish(1000);
    explosionPulseLightBehaviour->setRadiusDiminishSpeed(0.5f);
    explosionPulseLightBehaviour->setIntensityDiminishSpeed(1.5f);

    auto explosionParticles = explosion->attachComponent<ParticleSystem>();
    explosionParticles->setSortingLayer(SortingLayers::particles);
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
            auto deltaTime = Tile2D::time().getDeltaTime() * 60.0f;

            Vecf pos = particle->getTransform().getPosition();
            particle->getTransform().setRotation(particle->getTransform().getRotation() + 1.0f * deltaTime);
            particle->getTransform().setPosition(pos + (particle->getVelocity() * deltaTime));
            particle->setOpacity(particle->getOpacity() - (0.01f * deltaTime));
            float newSize = particle->getTransform().getScale().x - 0.007f * deltaTime;
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

    return explosion;
}

void Prefabs::pulseLight(Vecf position) {
    auto obj = Tile2D::createGameObject();
    obj->transform().setPosition(position);

    auto light = obj->attachComponent<PointLight>();
    light->setRadius(80.0);
    light->setIntensity(1.0);

    auto pulseLightBehaviour = obj->attachComponent<PulseLightBehaviour>();
    pulseLightBehaviour->getTimer().setInterval(1000);
}

void Prefabs::sparkle(Vecf position, Vecf velocity, Color color) {
    auto sparkle = Tile2D::createGameObject();
    sparkle->transform().setPosition(position);
    sparkle->transform().setRotation(velocity.angle());

    auto sparkleBody = sparkle->attachComponent<Body>();
    sparkleBody->setMass(10.0);
    sparkleBody->setVelocity(velocity);

    auto sparkleSprite = sparkle->attachComponent<Sprite>();
    sparkleSprite->setRect({-20, -20, 20, 20});
    sparkleSprite->setColor(color);
    sparkleSprite->setTexturePtr(Tile2D::resources().textures["light"]);

    auto sparkleBehaviour = sparkle->attachComponent<SparkleBehaviour>();
}

void Prefabs::sparkles(Vecf position, Vecf normal, Color color) {
    Vecf& n = normal;
    Vecf perp = Vecf(n.y, -n.x);

    float div = 100.0;
    auto random = Vecf(rand() % 100, rand() % 100) / div;

    sparkle(position, (n + random) * 100, color);
    sparkle(position, (perp + n + random) * 100, color);
    sparkle(position, ((perp * -1 + random) + n) * 100, color);
    sparkle(position, (perp + random) * 100, color);
    sparkle(position, (perp + random) * -100, color);
}

GameObject *Prefabs::background(Rect area, const char *texture, Color color) {
    auto background = Tile2D::createGameObject();
    auto bg = background->attachComponent<Background>();
    bg->setRatio(0.5f);
    bg->setTexturePtr(Tile2D::resources().textures[texture]);
    bg->setColor(color);
    bg->setOpacity(0.0f);
    bg->setSortingLayer(SortingLayers::background);
    auto bgBehaviour = background->attachComponent<BackgroundBehaviour>();
    bgBehaviour->setArea(area);
    return background;
}

GameObject *Prefabs::spawner(Rect area, GameObject* target, GameObject* (*spawnfunction)()) {
    GameObject* spawnerObj = Tile2D::createGameObject();
    auto spawnerBehaviour = spawnerObj->attachComponent<SpawnerBehaviour>();
    spawnerBehaviour->setTarget(target);
    spawnerBehaviour->setSpawnFunction(spawnfunction);
    spawnerBehaviour->setAreaRect(area);
    spawnerBehaviour->setInnerRect({-600.0f, -400.0f, 600.0f, 400.0f});
    spawnerBehaviour->setOuterRect({-1000.0f, -800.0f, 1000.0f, 800.0f});

    return spawnerObj;
}

GameObject *Prefabs::hud(GameObject* player) {
    auto hud = Tile2D::createGameObject();
    auto hudBehaviour = hud->attachComponent<HUD>();
    hudBehaviour->setPlayer(player);
}

