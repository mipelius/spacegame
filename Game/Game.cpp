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

#include "precompile.h"
#include "Game.h"

#include "Node.h"
#include "RouteGenerator.h"

#include "Camera.h"
#include "Background.h"
#include "Texture.h"
#include "MapTexture.h"
#include "Map.h"
#include "GameWorld.h"
#include "Renderer.h"

#include "EntityCollisionEventArgs.h"
#include "EntityCollisionEventHandler.h"
#include "Event.h"

#include "Spaceship.h"
#include "WalkingCreature.h"
#include "Team.h"

#include "CpuController.h"
#include "HumanController.h"
#include "Brains.h"
#include "NavigatorBrainCell.h"
#include "TargetSelectorBrainCell.h"

Game::Game() {
    // --- INITIALIZE RENDERER ---

    renderer = new Renderer();

    renderer->init(0, 0, 1200, 800, false);

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

	std::string filenames[] = {
			"images/green_block.bmp",
			"images/red_block.bmp",
			"images/blue_block.bmp",
			};
	MapTexture *mapTexture = new MapTexture(10, 10,	3, filenames);

    // --- WORLD ---

    map = new Map("images/map.bmp", mapTexture, 10, 10);
    world = new GameWorld(Vector(0, 9.81), 0.20, 0.001);
    world->setMap(map);
    renderer->setGameWorld(world);

    // --- PLAYER ---

    player = new Spaceship(Point(4500, 8500), 1000, 1);
    player->setShootingSpeed(10);
    playerController = new HumanController();
    playerController->setControllableObject(player);
    playerTeam = new Team("Team America ;)");
    player->setTeam(playerTeam);
    world->addEntity(player);

    // --- EXTERNAL PLAYER ---

    Spaceship* playerExt = new Spaceship(Point(4300, 8500), 1000, 1);
    //playerController->setControllableObject(player);
    playerExt->setTeam(playerTeam);
    world->addEntity(playerExt);

    // --- CAMERA ---

    Camera* camera = renderer->getCamera();
    camera->follow(player);

    // --- ENEMIES ---

    enemyTeam = new Team("Team Bastards");

    enemies = new std::list<Spaceship*>;
    for (int i=0; i<20; i++) {
        Spaceship* enemy = new Spaceship(Point(4500 + i * 50, 8700), 20, 1);
        enemy->setShootingSpeed(10);

        // brains and controller

        Brains* brains = new Brains();
        brains->addEnemyTeam(playerTeam);

        new NavigatorBrainCell(1.0, brains);
        new TargetSelectorBrainCell(2.0, brains, 1000);

        CpuController* controller = new CpuController(brains);
        controller->setControllableObject(enemy);

        // set team

        enemy->setTeam(enemyTeam);

        world->addEntity(enemy);
        enemies->push_back(enemy);
    }

    // --- BOSS ---

    boss = new Spaceship(Point(4500, 8700), 300, 3);
    boss->setShootingSpeed(4);
}

void Game::launch() {
    const Uint8* keys;
    Uint32 timeMilliSec = 0;

    Point routeStartPoint = Point(0, 0);
    Point routeGoalPoint = Point(0, 0);
    Node* route = nullptr;



    while (!SDL_QuitRequested()) {
        keys = SDL_GetKeyboardState(0);

        if (keys[SDL_SCANCODE_F1]) renderer->toggleCollisionShapesVisibility();
        if (keys[SDL_SCANCODE_F2]) renderer->toggleMapCollisionAreaVisibility();

        if (!player->isDead()) {
            if (keys[SDL_SCANCODE_LEFT]) playerController->left();
            if (keys[SDL_SCANCODE_RIGHT]) playerController->right();
            if (keys[SDL_SCANCODE_UP]) playerController->up();
            if (keys[SDL_SCANCODE_SPACE]) playerController->space();
        }

        for(std::list<Spaceship*>::iterator it = enemies->begin(); it != enemies->end(); it++) {
            if ((*it)->isDead()) {
                GameEntity* currentEntity = (*it);
                enemies->erase(it);
                it++;
                delete (currentEntity);
                continue;
            }
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

        // additional rendering

        // health

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

        // route

        glColor3f(0, 1, 0);
        glRectf(
                routeStartPoint.x-1 - renderer->getCamera()->getLocation().x,
                routeStartPoint.y-1 - renderer->getCamera()->getLocation().y,
                routeStartPoint.x+1 - renderer->getCamera()->getLocation().x,
                routeStartPoint.y+1 - renderer->getCamera()->getLocation().y
        );

        glColor3f(1, 0, 0);
        glRectf(
                routeGoalPoint.x-1 - renderer->getCamera()->getLocation().x,
                routeGoalPoint.y-1 - renderer->getCamera()->getLocation().y,
                routeGoalPoint.x+1 - renderer->getCamera()->getLocation().x,
                routeGoalPoint.y+1 - renderer->getCamera()->getLocation().y
        );

        if (route != nullptr) {
            Node* currentNode = route;

            glColor3f(0, 0, 1);
            glBegin(GL_LINE_STRIP);

            while (currentNode->getNextNode() != nullptr) {
                Point location = currentNode->getLocation();

                glVertex2f(
                        location.x - renderer->getCamera()->getLocation().x,
                        location.y - renderer->getCamera()->getLocation().y
                );

                currentNode = currentNode->getNextNode();
            }

            glEnd();
        }

        renderer->glSwap();
    }
}

Game::~Game() {
    delete renderer;
    delete map;
    delete world;
    delete player;
}