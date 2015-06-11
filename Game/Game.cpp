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

#include "SpriteContainer.h"
#include "DrawableMap.h"
#include "MapTexture.h"
#include "Camera.h"

#include "Spaceship.h"

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
#include "PointLight.h"

#include "Sprite.h"
#include "SpriteContainer.h"
#include "LightObject.h"
#include "DynamicLightObject.h"
#include "Sample.h"
#include "SamplePlayer.h"
#include "Ears.h"

#include "AnimatedTexture.h"
#include "Bomb.h"
#include "Explosion.h"
#include "Font.h"
#include "Text.h"

Game* Game::game_ = nullptr;

Game::Game() {
    isInitialized_ = false;
}

void Game::launch() {
    Text* text = new Text(App::getResources()->other->smallFont);
    text->location->set(Point(4000, 8000));
    text->string->set("Welcome to Space Game!");
    text->size->set(3);

    externalCanvas_->addDrawable(text);

    App::getMusicPlayer()->play(App::getResources()->tunes->spacegame);

    const Uint8* keys;
    Uint32 timeMilliSec = 0;
    Uint32 lightAddingInterval = 200; // ms
    Uint32 timePassedAfterLastLightAdd = 0;

    Uint32 shootingInterval = 200;
    Uint32 timePassedAfterLastShot = 0;
    Uint32 bombingInterval = 200;
    Uint32 timePassedAfterLastBomb = 0;

    Uint32 inventoryInterval = 300;
    Uint32 timePassedAfterLastInventoryToggle = 0;

    Uint32 mouseState = 0;
    Uint32 previousMouseState = 0;

    while (!SDL_QuitRequested()) {
        /// --- INPUT READING AND HANDLING ---

        keys = SDL_GetKeyboardState(0);

        spaceship_->body->torque->set(0);

        if (keys[SDL_SCANCODE_TAB]) {
            if (timePassedAfterLastInventoryToggle > inventoryInterval) {
                inventory_->canvas.isVisible->toggle();
                timePassedAfterLastInventoryToggle = 0;
            }
        }

        if (keys[SDL_SCANCODE_LEFT]) {
            if (spaceship_->body->angularVelocity->get() > -10) {
                spaceship_->body->torque->set(-250);
            }
        }
        if (keys[SDL_SCANCODE_RIGHT]) {
            if (spaceship_->body->angularVelocity->get() < 10) {
                spaceship_->body->torque->set(250);
            }
        }
        if (keys[SDL_SCANCODE_UP]) {
            spaceship_->accelerate();
        }

        if (keys[SDL_SCANCODE_SPACE]) {
            if (timePassedAfterLastShot > shootingInterval) {
                timePassedAfterLastShot = 0;
                spaceship_->shoot();
            }
        }

        if (keys[SDL_SCANCODE_X]) {
            if (timePassedAfterLastLightAdd > lightAddingInterval) {
                timePassedAfterLastLightAdd = 0;

                LightObject * light = new LightObject(spaceship_->location->get(), 400);

                canvas_->addDrawable(light->spriteContainer);
                shadowMask_->addLight(light->pointLight);

                Plot* plot = new Plot();
                plot->location->set(spaceship_->body->location->get());
                plot->size->set(1.0);

                smallMapCanvas_->addDrawable(plot);
            }
        }

        if (keys[SDL_SCANCODE_A]) {
            if (timePassedAfterLastBomb > bombingInterval) {
                timePassedAfterLastBomb = 0;

                Bomb* bomp = new Bomb();
                bomp->body->location->set(
                    spaceship_->location->get() + Vector::byAngle(
                        spaceship_->body->angle->get(), -20.0
                    )
                );

                // initial speed

                Vector speed = spaceship_->body->speed->get();
                if (speed.y <= 0) {
                    speed = speed * 0.25;
                    speed.y = 0;
                }
                else {
                    speed.x *= 0.25;
                    speed.y *= 0.5;
                }

                bomp->body->speed->set(speed);

                world_->add(bomp->body);
                canvas_->addDrawable(bomp->spriteContainer);
            }
        }

        if (keys[SDL_SCANCODE_C]) {
            int xx = spaceship_->location->get().x;
            int yy = spaceship_->location->get().y;

            for (int i = -10; i<=10; i++) {
                for (int j = -10; j<=10; j++) {
                    map_->setValueScaled(Point(xx + 10 * i, yy + 10 * j), App::getResources()->other->blockMapping->getBlock(0));
                }
            }
        }

        if (keys[SDL_SCANCODE_RETURN]) {
            smallMapCanvas_->isVisible->toggle();
        }

        int x, y;

        mouseState = SDL_GetMouseState(&x, &y);

        if (mouseState == 1) {
            inventory_->mouseButtonDown(x, y);
        }

        if (mouseState == 4) {
            // ---
        }

        if (mouseState == 0 && previousMouseState != 0) {
            inventory_->mouseButtonReleased(x, y);
        }

        previousMouseState = mouseState;

        /// --- PHYSICS ---

        Uint32 timeElapsedMilliSec = SDL_GetTicks() - timeMilliSec;
        timeMilliSec = SDL_GetTicks();
        world_->step(timeElapsedMilliSec / 1000.0);

        timePassedAfterLastLightAdd +=  timeElapsedMilliSec;
        timePassedAfterLastBomb += timeElapsedMilliSec;
        timePassedAfterLastShot += timeElapsedMilliSec;
        timePassedAfterLastInventoryToggle += timeElapsedMilliSec;

        /// --- ANIMATION ---

        App::getAnimationManager()->update(timeElapsedMilliSec / 1000.0);

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
    canvas_->w->set(Canvas::SIZE_MAX_WIDTH);
    canvas_->h->set(Canvas::SIZE_MAX_HEIGHT);
    canvas_->anchor->set(Canvas::Anchor::TOP_RIGHT);

    externalCanvas_ = new Canvas();
    externalCanvas_->setMargin(0, 0, 0, 0);
    externalCanvas_->w->set(Canvas::SIZE_MAX_WIDTH);
    externalCanvas_->h->set(Canvas::SIZE_MAX_HEIGHT);
    externalCanvas_->anchor->set(Canvas::Anchor::TOP_RIGHT);

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
    camera_->boundsRect->set(Rect(0, 0, 20000, 20000));
    camera_->areaRect->set(Rect(0, 0, App::getWindow()->w->get(), App::getWindow()->h->get()));

    canvas_->setCamera(camera_);
    externalCanvas_->setCamera(camera_);

    // --- PLAYER ---

    spaceship_ = new Spaceship();
    spaceship_->body->location->set(Point(4000, 8000));
    spaceship_->body->location->bind(spaceship_->location);

    world_->add(spaceship_->body);
    canvas_->addDrawable(spaceship_->spriteContainer);

    camera_->location->bind(spaceship_->body->location);

    Ears* ears = new Ears();
    ears->maxDistance->set(1400);
    ears->location->bind(spaceship_->location);
    App::getSamplePlayer()->setEars(ears);

    // --- SHADOW MASK  ---

    shadowMask_ = new ShadowMask(
            App::getWindow()->w->get(),
            App::getWindow()->h->get(),
            map_
    );

    canvas_->addShadowMask(shadowMask_);
    shadowMask_->ambientLight->set(0.00);

    PointLight* light = new PointLight(Point(4000, 9000), 400, true);
    shadowMask_->addLight(light);
    light->location->bind(spaceship_->location);

    // --- SMALL MAP ---

    smallMapCanvas_ = new Canvas();
    smallMapCanvas_->setMargin(20, 20, 20, 20);
    smallMapCanvas_->w->set(250);
    smallMapCanvas_->h->set(200);
    smallMapCanvas_->anchor->bind(canvas_->anchor);
    smallMapCanvas_->isBoundsVisible->set(true);
    smallMapCanvas_->opacity->set(0.5);

    smallMapCamera_ = new Camera();
    smallMapCamera_->boundsRect->set(Rect(0, 0, 20000, 20000));
    smallMapCamera_->areaRect->set(Rect(0, 0, 4000, 3000));
    smallMapCamera_->location->bind(spaceship_->body->location);
    smallMapCanvas_->setCamera(smallMapCamera_);

    smallMapCanvas_->addDrawable(drawableMap);
    Plot* plot = new Plot();
    plot->location->bind(spaceship_->body->location);
    plot->size->set(1.0);
    smallMapCanvas_->addDrawable(plot);

    canvas_->addComponent(smallMapCanvas_);

    // --- INVENTORY ---

    inventory_ = new Inventory();
    canvas_->addComponent(&inventory_->canvas);

    // --- ANIMATIONS ---

    //

    isInitialized_ = true;
}

Canvas *Game::getExternalCanvas() {
    return externalCanvas_;
}
