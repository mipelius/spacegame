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
#include "Window.h"
#include "Canvas.h"

#include "WorldMap.h"
#include "BlockMapping.h"
#include "Body.h"
#include "PhysicsWorld.h"

#include "DrawableGroup.h"
#include "DrawableMap.h"
#include "MapTexture.h"
#include "Camera.h"

#include "Music.h"
#include "MusicPlayer.h"

#include "GuiComponentBase.h"
#include "Background.h"
#include "Texture.h"
#include "Plot.h"
#include "json.h"
#include "JsonFileManager.h"

#include "AnimationManager.h"
#include "ShadowMask.h"
#include "SamplePlayer.h"
#include "Ears.h"

#include "AnimatedTexture.h"
#include "Font.h"
#include "Text.h"

Game* Game::game_ = nullptr;

Game::Game() {
    isInitialized_ = false;
}

void Game::launch() {
    App::getMusicPlayer()->play(App::getResources()->tunes->spacegame);

    const Uint8* keys;

    while (!SDL_QuitRequested()) {
        /// --- INPUT READING AND HANDLING ---

        keys = SDL_GetKeyboardState(0);

        if (keys[SDL_SCANCODE_1]) {
            inventory_->selectSlot(1);
        }

        if (keys[SDL_SCANCODE_2]) {
            inventory_->selectSlot(2);
        }

        if (keys[SDL_SCANCODE_3]) {
            inventory_->selectSlot(3);
        }

        if (keys[SDL_SCANCODE_4]) {
            inventory_->selectSlot(4);
        }

        if (keys[SDL_SCANCODE_5]) {
            inventory_->selectSlot(5);
        }

        if (keys[SDL_SCANCODE_LEFT]) {

        }

        if (keys[SDL_SCANCODE_RIGHT]) {

        }

        if (keys[SDL_SCANCODE_UP]) {

        }

        if (keys[SDL_SCANCODE_SPACE]) {

        }

        if (keys[SDL_SCANCODE_X]) {

        }

        if (keys[SDL_SCANCODE_RETURN]) {

        }

        inventory_->checkMouseActions();

        /// --- PHYSICS ---

//        Uint32 timeElapsedMilliSec = SDL_GetTicks() - timeMilliSec;
//        timeMilliSec = SDL_GetTicks();
//        world_->step(timeElapsedMilliSec / 1000.0);   //TODO

        /// --- ANIMATION ---

//        App::getAnimationManager()->update(timeElapsedMilliSec / 1000.0);

        /// --- RENDERING ---

        App::getWindow()->update();
    }
}

Game::~Game() {
    delete map_;
    delete world_;
}

Game *Game::getInstance() {
    if (!game_) {
        game_ = new Game();
    }

    return game_;
}

PhysicsWorld *Game::getWorld() {
    return world_;
}

WorldMap *Game::getMap() {
    return map_;
}

Canvas *Game::getCanvas() {
    return canvas_;
}

ShadowMask *Game::getShadowMask() {
    return shadowMask_;
}

void Game::initialize() {
    // --- CANVAS ---

    canvas_ = new Canvas();
    canvas_->setMargin(0, 0, 0, 0);
    canvas_->w.set(Canvas::SIZE_MAX_WIDTH);
    canvas_->h.set(Canvas::SIZE_MAX_HEIGHT);
    canvas_->anchor.set(Canvas::Anchor::TOP_RIGHT);

    externalCanvas_ = new Canvas();
    externalCanvas_->setMargin(0, 0, 0, 0);
    externalCanvas_->w.set(Canvas::SIZE_MAX_WIDTH);
    externalCanvas_->h.set(Canvas::SIZE_MAX_HEIGHT);
    externalCanvas_->anchor.set(Canvas::Anchor::TOP_RIGHT);

    canvas_->addComponent(externalCanvas_);

    Background* background = new Background();
    background->setRatio(0.5);
    background->setTexture(App::getResources()->textures->bg2);

    canvas_->addDrawable(background);

    App::getWindow()->addComponent(canvas_);

    // --- WORLD & MAP ---

    map_ = new WorldMap(
            "images/map.bmp",
            App::getResources()->other->blockMapping,
            10,
            10
    );

    world_ = new PhysicsWorld(Vector(0, 9.81), 0.20, 0.0001);
    world_->setMap(map_);

    DrawableMap* drawableMap = new DrawableMap();
    drawableMap->setMap(map_);
    drawableMap->setMapTexture(App::getResources()->other->blockMapping->getMapTexture());

    canvas_->addDrawable(drawableMap);

    // --- CAMERA ---

    camera_ = new Camera();
    camera_->boundsRect.set(Rect(0, 0, 20000, 20000));
    camera_->areaRect.set(Rect(0, 0, App::getWindow()->w.get(), App::getWindow()->h.get()));

    canvas_->setCamera(camera_);
    externalCanvas_->setCamera(camera_);

    // --- SHADOW MASK  ---

    shadowMask_ = new ShadowMask(
            App::getWindow()->w.get(),
            App::getWindow()->h.get(),
            map_
    );

    canvas_->addShadowMask(shadowMask_);
    shadowMask_->ambientLight.set(0.05);

    // --- SMALL MAP ---

    smallMapCanvas_ = new Canvas();
    smallMapCanvas_->setMargin(20, 20, 20, 20);
    smallMapCanvas_->w.set(250);
    smallMapCanvas_->h.set(200);
    smallMapCanvas_->anchor.bind(canvas_->anchor);
    smallMapCanvas_->isBoundsVisible.set(true);
    smallMapCanvas_->opacity.set(0.5);

    smallMapCamera_ = new Camera();
    smallMapCamera_->boundsRect.set(Rect(0, 0, 20000, 20000));
    smallMapCamera_->areaRect.set(Rect(0, 0, 4000, 3000));
    smallMapCanvas_->setCamera(smallMapCamera_);

    smallMapCanvas_->addDrawable(drawableMap);
    Plot* plot = new Plot();
    plot->size.set(1.0);
    smallMapCanvas_->addDrawable(plot);

    canvas_->addComponent(smallMapCanvas_);

    // --- GUI CANVAS ---

    GUICanvas_ = new Canvas();
    GUICanvas_->setMargin(0, 0, 0, 0);
    GUICanvas_->w.set(Canvas::SIZE_MAX_WIDTH);
    GUICanvas_->h.set(Canvas::SIZE_MAX_HEIGHT);
    GUICanvas_->anchor.set(Canvas::Anchor::TOP_RIGHT);

    Camera* GUICanvasCamera = new Camera();
    GUICanvasCamera->boundsRect.set(
            Rect(
                    0,
                    0,
                    App::getWindow()->w.get(),
                    App::getWindow()->h.get()
            )
    );
    GUICanvasCamera->areaRect.set(
            Rect(
                    0,
                    0,
                    App::getWindow()->w.get(),
                    App::getWindow()->h.get()
            )
    );

    GUICanvas_->setCamera(GUICanvasCamera);

    App::getWindow()->addComponent(GUICanvas_);

    // --- INVENTORY ---

    inventory_ = new Inventory();
    inventory_->location.set(Point(20, 20));
    GUICanvas_->addDrawable(inventory_);

    isInitialized_ = true;
}

Canvas *Game::getExternalCanvas() {
    return externalCanvas_;
}
