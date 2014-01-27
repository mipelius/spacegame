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

    // --- TEAMS ---

    playerTeam = new Team("Team America ;)");
    enemyTeam = new Team("Team Bastards");

    // --- PLAYER ---

    player = new Spaceship(Point(4500, 8500), 1000, 1);
    player->setShootingSpeed(10);
    playerController = new HumanController();
    playerController->setControllableObject(player);
    player->setTeam(playerTeam);
    world->addEntity(player);

    // --- CAMERA ---

    Camera* camera = renderer->getCamera();
    camera->follow(player);

    // --- OTHER SPACESHIPS ---

    otherSpaceships = new std::list<Spaceship*>;
    for (int i=0; i<20; i++) {
        bool isPlayerTeam = (bool)(i % 2);

        Spaceship* spaceship = new Spaceship(Point(4000 + i * 100, 8700 - rand() % 1000), 20, isPlayerTeam ? 1 : 2);
        spaceship->setShootingSpeed(10);

        // brains and controller

        Brains* brains = new Brains();
        brains->addEnemyTeam(isPlayerTeam ? enemyTeam : playerTeam);

        brains->addCell(new NavigatorBrainCell(0.01));
        brains->addCell(new TargetSelectorBrainCell(0.01, 500.0));

        CpuController* controller = new CpuController(brains);
        controller->setControllableObject(spaceship);

        // set team

        spaceship->setTeam(isPlayerTeam ? playerTeam : enemyTeam);

        world->addEntity(spaceship);
        otherSpaceships->push_back(spaceship);
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

        renderer->glSwap();
    }
}

Game::~Game() {
    delete renderer;
    delete map;
    delete world;
    delete player;
}