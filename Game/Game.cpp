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

#include "App.h"

#include "Node.h"
#include "RouteGenerator.h"
#include "RouteResponse.h"
#include "RouteRequest.h"

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
#include "RouteGeneratorBrainCell.h"
#include "EyeBrainCell.h"

#include "MusicPlayer.h"
#include "Music.h"
#include "SamplePlayer.h"
#include "Sample.h"

#include "GameArea.h"

Game::Game() {
    // --- INITIALIZE RENDERER ---

    renderer = new Renderer();

    renderer->init(0, 0, 1920, 1200, true);

    renderer->setSmallMap(Rect(Point(10, 10), Point(260, 260)), 2500, 2500);

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
    world->getRouteGenerator()->setMaxGeneratingTimeMilliSec(5);
    renderer->setGameWorld(world);

    // --- GAME AREAS --- //

    this->gameAreas = new std::list<GameArea*>();

    int areasHorizontally = 2;
    int areasVertically = 2;
    double areaW = world->getW() / areasHorizontally;
    double areaH = world->getH() / areasVertically;

    Texture* bgTexture1 = new Texture("images/bg1.jpg");
    Music* music1 = new Music("music/testing_reversed.mp3");

    Texture* bgTexture2 = new Texture("images/bg2.jpg");
    Music* music2 = new Music("music/testing.mp3");

    for (int x = 0; x < areasHorizontally; x++) {
        for (int y = 0; y < areasVertically; y++) {
            int areaType = (x % 2 + (y + 1) % 2) % 2;

            GameArea* area;
            Background* background;

            double x1 = ((double)x / areasHorizontally) * world->getW();
            double y1 = ((double)y / areasHorizontally) * world->getH();
            double x2 = x1 + areaW;
            double y2 = y1 + areaH;

            switch (areaType) {
                case 0 :
                    background = new Background(bgTexture1, x1, y1);
                    area = new GameArea(background, music1, Rect(Point(x1, y1), Point(x2, y2)));
                    break;
                case 1 :
                    background = new Background(bgTexture2, x1, y1);
                    area = new GameArea(background, music2, Rect(Point(x1, y1), Point(x2, y2)));
                    break;
                default :
                    area = nullptr;
                    break;
            }

            if (area) {
                gameAreas->push_back(area);
            }

        }
    }

    // --- TEAMS ---

    playerTeam = new Team("Team America ;)");
    enemyTeam = new Team("Team Bastards");

    // --- PLAYER ---

    player = new Spaceship(Point(4500, 9000), 1000, 1);
    player->setShootingSpeed(10);
    playerController = new HumanController();
    playerController->setControllableObject(player);
    player->setTeam(playerTeam);
    world->addEntity(player);

    updateCurrentGameArea(player->getLocation());

    // --- CAMERA ---

    Camera* camera = renderer->getCamera();
    camera->follow(player);

    // --- OTHER SPACESHIPS ---

//    otherSpaceships = new std::list<Spaceship*>;
//    for (int i=0; i<20; i++) {
//        double x = 4000 + (i % 20) * 64;
//        double y = 8700 - rand() % 500 - (i / 20) * 500;
//
//        Spaceship* spaceship = new Spaceship(Point(x, y), 20, 1);
//        spaceship->setShootingSpeed(10);
//
//        // brains
//        Brains* brains = new Brains();
//        brains->addEnemyTeam(playerTeam);
//
//        // brain cell: target selector
//        TargetSelectorBrainCell* targetSelectorBrainCell = new TargetSelectorBrainCell(0.5, 600.0);
//        brains->addCell(targetSelectorBrainCell);
//
//        // brain cell: eye
//        EyeBrainCell* eyeBrainCell = new EyeBrainCell(0.1);
//        eyeBrainCell->setTargetSelectorBrainCell(targetSelectorBrainCell);
//        brains->addCell(eyeBrainCell);
//
//        // brain cell: route generator
//        RouteGeneratorBrainCell* routeGeneratorBrainCell = new RouteGeneratorBrainCell(1.0);
//        brains->addCell(routeGeneratorBrainCell);
//
//        // brain cell: navigator
//        NavigatorBrainCell* navigatorBrainCell = new NavigatorBrainCell(0.01);
//        navigatorBrainCell->setEyeBrainCell(eyeBrainCell);
//        navigatorBrainCell->setRouteGeneratorBrainCell(routeGeneratorBrainCell);
//        brains->addCell(navigatorBrainCell);
//
//        // controller
//        CpuController* controller = new CpuController(brains);
//        controller->setControllableObject(spaceship);
//
//        // set team
//        spaceship->setTeam(enemyTeam);
//
//        world->addEntity(spaceship);
//        otherSpaceships->push_back(spaceship);
//    }

    // --- BOSS ---

    boss = new Spaceship(Point(4500, 8700), 300, 3);
    boss->setShootingSpeed(4);
}

void Game::launch() {
    const Uint8* keys;
    Uint32 timeMilliSec = 0;


    while (!SDL_QuitRequested()) {
        /// --- GAME AREA UPDATING ---

        updateCurrentGameArea(player->getLocation());

        /// --- INPUT READING AND HANDLING ---

        keys = SDL_GetKeyboardState(0);

        if (keys[SDL_SCANCODE_F1]) renderer->toggleCollisionShapesVisibility();
        if (keys[SDL_SCANCODE_F2]) renderer->toggleMapCollisionAreaVisibility();

        if (!player->isDead()) {
            if (keys[SDL_SCANCODE_LEFT]) playerController->left();
            if (keys[SDL_SCANCODE_RIGHT]) playerController->right();
            if (keys[SDL_SCANCODE_UP]) playerController->up();
            if (keys[SDL_SCANCODE_SPACE]) playerController->space();
        }

        /// --- PHYSICS ---

        Uint32 timeElapsedMilliSec = SDL_GetTicks() - timeMilliSec;
        world->step(timeElapsedMilliSec / 1000.0);
        timeMilliSec = SDL_GetTicks();

        /// --- RENDERING ---

        renderer->render();

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

void Game::updateCurrentGameArea(Point point) {
    for(std::list<GameArea*>::iterator it = gameAreas->begin(); it != gameAreas->end(); it++) {
        Rect rect = (*it)->getRect();
        if (point.isIn(rect)) {
            if ((*it) != currentGameArea) {
                currentGameArea = (*it);

                renderer->setBackground(currentGameArea->getBackground());
                App::instance()->getMusicPlayer()->play(currentGameArea->getMusic());
            }

            return;
        }
    }

    // if there is no area in the given point

    currentGameArea = nullptr;
    renderer->setBackground(nullptr);
    App::instance()->getMusicPlayer()->stop();
}
