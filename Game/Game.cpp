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
#include "BackgroundRenderer.h"
#include "Texture.h"
#include "MapTexture.h"
#include "Map.h"
#include "GameWorld.h"

#include "CoreRenderer.h"
#include "BackgroundRenderer.h"
#include "GameWorldRenderer.h"
#include "MapRenderer.h"
#include "MiniMapRenderer.h"

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
    // --- WINDOW PROPERTIES

    int x = 100;
    int y = 100;
    int w = 1280;
    int h = 800;

    bool enableFullScreen = false;

    // --- WORLD ---

    map_ = new Map("images/map.bmp", 10, 10);
    world_ = new GameWorld(Vector(0, 9.81), 0.20, 0.001);
    world_->setMap(map_);
    world_->getRouteGenerator()->setMaxGeneratingTimeMilliSec(5);

    // --- INITIALIZE RENDERER ---

    renderer_ = new CoreRenderer();
    renderer_->initialize(x, y, w, h, enableFullScreen);

    Rect renderingAreaRect = Rect(0, 0, w, h);
    Rect cameraBoundsRect = Rect(0, 0, world_->getW(), world_->getH());
    camera_ = new Camera(renderingAreaRect, cameraBoundsRect);
    renderer_->setCamera(camera_);

    // BackgroundRenderer

    backgroundRenderer_ = new BackgroundRenderer(renderingAreaRect);
    backgroundRenderer_->setRatio(0.3);
    renderer_->addRenderer(backgroundRenderer_);

    // MapRenderer
    MapRenderer* mapRenderer = new MapRenderer(renderingAreaRect);
    mapRenderer->setMap(map_);
    std::string filenames[] = {
            "images/green_block.bmp",
            "images/red_block.bmp",
            "images/blue_block.bmp",
    };
    MapTexture *mapTexture = new MapTexture(10, 10,	3, filenames);
    mapRenderer->setMapTexture(mapTexture);
    renderer_->addRenderer(mapRenderer);

    // GameWorldRenderer

    GameWorldRenderer* gameWorldRenderer = new GameWorldRenderer(renderingAreaRect);
    gameWorldRenderer->setGameWorld(world_);
    renderer_->addRenderer(gameWorldRenderer);

    // MiniMapRenderer

    MiniMapRenderer* miniMapRenderer = new MiniMapRenderer(Rect(10, 10, 310, 310));
    miniMapCamera_ = new Camera(Rect(0, 0, 3000, 3000), cameraBoundsRect);
    miniMapRenderer->setCamera(miniMapCamera_);
    miniMapRenderer->setMap(map_);
    renderer_->addRenderer(miniMapRenderer);

    // --- GAME AREAS --- //

    this->gameAreas = new std::list<GameArea*>();

    int areasHorizontally = 2;
    int areasVertically = 2;
    double areaW = world_->getW() / areasHorizontally;
    double areaH = world_->getH() / areasVertically;

    Texture* bgTexture1 = new Texture("images/bg1.jpg");
    Music* music1 = new Music("music/testing_reversed.mp3");

    Texture* bgTexture2 = new Texture("images/bg2.jpg");
    Music* music2 = new Music("music/testing.mp3");

    for (int x = 0; x < areasHorizontally; x++) {
        for (int y = 0; y < areasVertically; y++) {
            int areaType = (x % 2 + (y + 1) % 2) % 2;

            GameArea* area;

            double x1 = ((double)x / areasHorizontally) * world_->getW();
            double y1 = ((double)y / areasHorizontally) * world_->getH();
            double x2 = x1 + areaW;
            double y2 = y1 + areaH;

            switch (areaType) {
                case 0 :
                    area = new GameArea(bgTexture1, music1, Rect(x1, y1, x2, y2));
                    break;
                case 1 :
                    area = new GameArea(bgTexture2, music2, Rect(x1, y1, x2, y2));
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
    world_->addEntity(player);

    updateCurrentGameArea(player->getLocation());

    // --- OTHER SPACESHIPS ---

    otherSpaceships = new std::list<Spaceship*>;
    for (int i=0; i<20; i++) {
        double x = 4000 + (i % 20) * 64;
        double y = 8700 - rand() % 500 - (i / 20) * 500;

        Spaceship* spaceship = new Spaceship(Point(x, y), 20, 1);
        spaceship->setShootingSpeed(10);

        // brains
        Brains* brains = new Brains();
        brains->addEnemyTeam(playerTeam);

        // brain cell: target selector
        TargetSelectorBrainCell* targetSelectorBrainCell = new TargetSelectorBrainCell(0.5, 600.0);
        brains->addCell(targetSelectorBrainCell);

        // brain cell: eye
        EyeBrainCell* eyeBrainCell = new EyeBrainCell(0.1);
        eyeBrainCell->setTargetSelectorBrainCell(targetSelectorBrainCell);
        brains->addCell(eyeBrainCell);

        // brain cell: route generator
        RouteGeneratorBrainCell* routeGeneratorBrainCell = new RouteGeneratorBrainCell(1.0);
        brains->addCell(routeGeneratorBrainCell);

        // brain cell: navigator
        NavigatorBrainCell* navigatorBrainCell = new NavigatorBrainCell(0.01);
        navigatorBrainCell->setEyeBrainCell(eyeBrainCell);
        navigatorBrainCell->setRouteGeneratorBrainCell(routeGeneratorBrainCell);
        brains->addCell(navigatorBrainCell);

        // controller
        CpuController* controller = new CpuController(brains);
        controller->setControllableObject(spaceship);

        // set team
        spaceship->setTeam(enemyTeam);

        world_->addEntity(spaceship);
        otherSpaceships->push_back(spaceship);
    }

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

        if (!player->isDead()) {
            if (keys[SDL_SCANCODE_LEFT]) playerController->left();
            if (keys[SDL_SCANCODE_RIGHT]) playerController->right();
            if (keys[SDL_SCANCODE_UP]) playerController->up();
            if (keys[SDL_SCANCODE_SPACE]) playerController->space();
        }

        /// --- PHYSICS ---

        Uint32 timeElapsedMilliSec = SDL_GetTicks() - timeMilliSec;
        world_->step(timeElapsedMilliSec / 1000.0);
        timeMilliSec = SDL_GetTicks();

        /// --- RENDERING ---

        camera_->setCenteredLocation(player->getLocation());
        miniMapCamera_->setCenteredLocation(player->getLocation());
        renderer_->render();

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

        renderer_->glSwap();
    }
}

Game::~Game() {
    delete renderer_;
    delete map_;
    delete world_;
    delete player;
}

void Game::updateCurrentGameArea(Point point) {
    for(std::list<GameArea*>::iterator it = gameAreas->begin(); it != gameAreas->end(); it++) {
        Rect rect = (*it)->getRect();
        if (point.isIn(rect)) {
            if ((*it) != currentGameArea) {
                currentGameArea = (*it);

                backgroundRenderer_->setBackgroundTexture(currentGameArea->getBackgroundTexture());
                App::instance()->getMusicPlayer()->play(currentGameArea->getMusic());
            }

            return;
        }
    }

    // if there is no area in the given point

    currentGameArea = nullptr;
    backgroundRenderer_->setBackgroundTexture(nullptr);
    App::instance()->getMusicPlayer()->stop();
}
