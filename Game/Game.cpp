// This file is part of SpaceGame.
// Copyright (C) 2014  Miika Pelkonen
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

#include "Game.h"
#include <iostream>
#include "../Core/Renderer.h"
#include "../Core/GameObjectGroup.h"
#include "../Event/CollisionEventArgs.h"
#include "../Event/CollisionEventHandler.h"
#include "../Primitives/Point.h"
#include "../Primitives/Vector.h"
#include "Spaceship.h"
#import "Missile.h"

void onCollision(GameEntity *entity, CollisionEventArgs* args) {
    if (args->otherEntity && args->otherEntity->getOwner() == entity) return;
    entity->setSpeed(entity->getSpeed() * 0.5);
    entity->setLocation(args->oldLocation);

    if ((int) entity->getAngularVelocity() != 0) {
        entity->applyTorque(-entity->getAngularVelocity() * 50);
    }
}

void onSpaceshipCollision(GameEntity *entity, CollisionEventArgs* args) {
    onCollision(entity, args);

    if (entity->getSpeed().length() < 10) {
        Spaceship* spaceship = dynamic_cast<Spaceship*>(entity);
        if (spaceship) {
            spaceship->setStuck();
        }
    }
}

SpaceGame::SpaceGame() {
    // --- INITIALIZE RENDERER ---

    renderer = new Renderer();
    renderer->init(0, 0, 1920, 1200, true);

    renderer->addBackground(
            new Background(
                    new Texture("images/bg1.jpg"),
                    0,
                    7200
            )
    );

    renderer->addBackground(
            new Background(
                    new Texture("images/bg2.jpg"),
                    3400,
                    7200
            )
    );

    MapTexture *mapTexture = new MapTexture(
            10,
            10,
            3,
            (std::string[]) {
                    "images/green_block.bmp",
                    "images/red_block.bmp",
                    "images/blue_block.bmp",
            }
    );

    // --- WORLD ---

    map = new Map("images/map.bmp", mapTexture, 10, 10);
    world = new GameWorld(Vector(0, 9.81), 0.20, 0.001);
    world->setMap(map);
    renderer->setGameWorld(world);

    // --- PLAYER ---

    player = new Spaceship(Point(4500, 8500), 1000, 1);
    player->getCollisionEvent()->add(new CollisionEventHandler(onSpaceshipCollision));
    player->setShootingSpeed(10);
    world->addEntity(player);

    // --- CAMERA ---

    Camera* camera = renderer->getCamera();
    camera->follow(player);

    // --- ENEMIES ---

    enemies = new std::list<Spaceship*>;
    for (int i=0; i<20; i++) {
        Spaceship* enemy = new Spaceship(Point(4500 + i * 50, 8700), 20, 1);
        enemy->getCollisionEvent()->add(new CollisionEventHandler(onSpaceshipCollision));
        enemy->setShootingSpeed(10);
        world->addEntity(enemy);
        enemies->push_back(enemy);
    }

    // --- BOSS ---

    boss = new Spaceship(Point(4500, 8700), 300, 3);
    boss->getCollisionEvent()->add(new CollisionEventHandler(onSpaceshipCollision));
    boss->setShootingSpeed(4);

}

void SpaceGame::launch() {
    const Uint8* keys;
    Uint32 timeMilliSec = 0;

    while (!SDL_QuitRequested()) {
        keys = SDL_GetKeyboardState(0);

        if (keys[SDL_SCANCODE_F1]) renderer->toggleCollisionShapesVisibility();
        if (keys[SDL_SCANCODE_F2]) renderer->toggleMapCollisionAreaVisibility();

        if (!player->isDead()) {
            if (keys[SDL_SCANCODE_LEFT]) if (player->getAngularVelocity() > -80) player->applyTorque(-359);
            if (keys[SDL_SCANCODE_RIGHT]) if (player->getAngularVelocity() < 80) player->applyTorque(359);
            if (keys[SDL_SCANCODE_UP]) {
                if (player->isStuck()) {
                    player->setLocation(player->getLocation() + Vector::byAngle(player->getAngle() - 90, 2));
                    player->setSpeed(Vector::byAngle(player->getAngle() - 90, 200));
                    player->setNotStuck();
                }
                player->applyForce(Vector::byAngle(player->getAngle() - 90, 120000));
            }

            if (keys[SDL_SCANCODE_SPACE]) player->shoot();
        }

        for(std::list<Spaceship*>::iterator it = enemies->begin(); it != enemies->end(); it++) {
            if ((*it)->isDead()) {
                GameEntity* currentEntity = (*it);
                enemies->erase(it);
                it++;
                delete (currentEntity);
                continue;
            }
            if (!(rand() % 300)) (*it)->shoot();
        }

        if (enemies->empty() && !(boss->getWorld()) && (!boss->isDead())) {
            world->addEntity(boss);
        }

        if (boss && boss->getWorld()) {
            if(boss->getSpeed().y > 0) boss->applyForce(Vector(0, -10000));
            boss->setAngle(boss->getAngle() + 1);
            boss->shoot();
            if (!(rand() % 100)) {
                WalkingCreature* creature = new WalkingCreature(boss->getLocation(), 100);
                creature->applyForce(Vector::byAngle(boss->getAngle() - 90, 20000));
                creature->setTarget(player);
                world->addEntity(creature);
            }
        }

        Uint32 timeElapsedMilliSec = SDL_GetTicks() - timeMilliSec;
        world->step(timeElapsedMilliSec / 1000.0);
        timeMilliSec = SDL_GetTicks();

        renderer->render();


        glDisable(GL_TEXTURE_2D);

        glColor3f(0.6, 0.0, 0.0);

        glRectf(1650, 20, 1650 + (250 * player->getHealth() / player->getMaxHealth()), 40);

        glColor3f(1.0, 1.0, 1.0);

        glBegin(GL_LINE_LOOP);
        glVertex2i(1650, 20);
        glVertex2i(1900, 20);
        glVertex2i(1900, 40);
        glVertex2i(1650, 40);
        glEnd();

        renderer->glSwap();
    }
}

SpaceGame::~SpaceGame() {
    delete renderer;
    delete map;
    delete world;
    delete player;
}