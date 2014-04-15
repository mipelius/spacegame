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

#include "MyGameObject.h"

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

#include "AnimatedTexture.h"
#include "Bomb.h"
#include "Explosion.h"

Game* Game::game_ = nullptr;

Game::Game() {
    isInitialized_ = false;
}

void Game::launch() {
    App::getInstance()->getMusicPlayer()->play(new Music("music/spacegame.mp3"));

    Sample* sample = new Sample("soundfx/spaceship_shoot.wav");

    const Uint8* keys;
    Uint32 timeMilliSec = 0;
    Uint32 lightAddingInterval = 200; // ms
    Uint32 timePassedAfterLastLightAdd = 0;

    while (!SDL_QuitRequested()) {
        /// --- INPUT READING AND HANDLING ---

        keys = SDL_GetKeyboardState(0);

        myGameObject_->body->torque->set(0);

        if (keys[SDL_SCANCODE_LEFT]) {
            if (myGameObject_->body->angularVelocity->get() > -10) {
                myGameObject_->body->torque->set(-250);
            }
        }
        if (keys[SDL_SCANCODE_RIGHT]) {
            if (myGameObject_->body->angularVelocity->get() < 10) {
                myGameObject_->body->torque->set(250);
            }
        }
        if (keys[SDL_SCANCODE_UP]) {
            myGameObject_->accelerate();
        }

        if (keys[SDL_SCANCODE_SPACE]) {
            if (timePassedAfterLastLightAdd > lightAddingInterval) {
                timePassedAfterLastLightAdd = 0;

                LightObject * light = new LightObject(myGameObject_->location->get(), 500);

                canvas_->addDrawable(light->spriteContainer);
                shadowMask_->addLight(light->pointLight);

                Plot* plot = new Plot();
                plot->location->set(myGameObject_->body->location->get());
                plot->size->set(1.0);

                smallMapCanvas_->addDrawable(plot);
            }
        }

        if (keys[SDL_SCANCODE_A]) {
            if (timePassedAfterLastLightAdd > lightAddingInterval) {
                timePassedAfterLastLightAdd = 0;

                Bomb* bomp = new Bomb();
                bomp->body->location->set(
                    myGameObject_->location->get() + Vector::byAngle(
                        myGameObject_->body->angle->get(), -20.0
                    )
                );

                // initial speed

                Vector speed = myGameObject_->body->speed->get();
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
            int xx = myGameObject_->location->get().x;
            int yy = myGameObject_->location->get().y;

            for (int i = -10; i<=10; i++) {
                for (int j = -10; j<=10; j++) {
                    map_->setValueScaled(Point(xx + 10 * i, yy + 10 * j), blockMapping_->getBlock(0));
                }
            }
        }

        if (keys[SDL_SCANCODE_RETURN]) {
            smallMapCanvas_->isVisible->toggle();
        }

        if (keys[SDL_SCANCODE_Z]) {
            double old = shadowMask_->ambientLight->get();
            shadowMask_->ambientLight->set(old - 0.01);
        }
        if (keys[SDL_SCANCODE_X]) {
            double old = shadowMask_->ambientLight->get();
            shadowMask_->ambientLight->set(old + 0.01);
        }

        int x, y;

        Uint32 value =  SDL_GetMouseState(&x, &y);

        if (value == 1) {
            int xx = x + camera_->areaRect->get().x1;
            int yy = y + camera_->areaRect->get().y1;

            for (int i = -1; i<=1; i++) {
                for (int j = -1; j<=1; j++) {
                    map_->setValueScaled(Point(xx + 10 * i, yy + 10 * j), blockMapping_->getBlock(255));
                }
            }
        }

        if (value == 4) {
            int xx = x + camera_->areaRect->get().x1;
            int yy = y + camera_->areaRect->get().y1;

            for (int i = -1; i<=1; i++) {
                for (int j = -1; j<=1; j++) {
                    map_->setValueScaled(Point(xx + 10 * i, yy + 10 * j), blockMapping_->getBlock(0));
                }
            }
        }

        /// --- PHYSICS ---

        Uint32 timeElapsedMilliSec = SDL_GetTicks() - timeMilliSec;
        timeMilliSec = SDL_GetTicks();
        world_->step(timeElapsedMilliSec / 1000.0);

        timePassedAfterLastLightAdd +=  timeElapsedMilliSec;

        /// --- ANIMATION ---

        App::getInstance()->getAnimationManager()->update(timeElapsedMilliSec / 1000.0);

        /// --- RENDERING ---

        App::getInstance()->getWindow()->update();
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

BlockMapping *Game::getBlockMapping() {
    return blockMapping_;
}

ShadowMask *Game::getShadowMask() {
    return shadowMask_;
}

void Game::initialize() {
    // --- WINDOW PROPERTIES ---

    int x = 0;
    int y = 0;
    int w = 1920;
    int h = 1200;

    bool enableFullScreen = false;

    Window* window = App::getInstance()->getWindow();
    window->initialize(x, y, w, h, enableFullScreen);

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

    Texture* texture = new Texture("images/bg2.jpg");
    Background* background = new Background();
    background->setRatio(0.5);
    background->setTexture(texture);

    canvas_->addDrawable(background);

    window->addComponent(canvas_);

    // --- WORLD & MAP ---

    blockMapping_ = new BlockMapping("json/blockMappings.json");

    map_ = new WorldMap("images/map.bmp", blockMapping_, 10, 10);
    world_ = new PhysicsWorld(Vector(0, 9.81), 0.20, 0.0001);
    world_->setMap(map_);

    DrawableMap* drawableMap = new DrawableMap();
    drawableMap->setMap(map_);
    drawableMap->setMapTexture(blockMapping_->getMapTexture());

    canvas_->addDrawable(drawableMap);

    // --- CAMERA ---

    camera_ = new Camera();
    camera_->boundsRect->set(Rect(0, 0, 20000, 20000));
    camera_->areaRect->set(Rect(0, 0, w, h));

    canvas_->setCamera(camera_);
    externalCanvas_->setCamera(camera_);

    // --- PLAYER ---

    myGameObject_ = new MyGameObject();
    myGameObject_->body->location->set(Point(4000, 8000));
    myGameObject_->body->location->bind(myGameObject_->location);

    world_->add(myGameObject_->body);
    canvas_->addDrawable(myGameObject_->spriteContainer);

    camera_->location->bind(myGameObject_->body->location);

    // --- SHADOW MASK  ---

    shadowMask_ = new ShadowMask(w, h, map_);
    canvas_->addShadowMask(shadowMask_);
    shadowMask_->ambientLight->set(0.05);

//    PointLight* light = new PointLight(Point(4000, 9000), 300);
//    shadowMask_->addLight(light);
//    light->location->bind(myGameObject_->location);

    // --- SMALL MAP ---

    smallMapCanvas_ = new Canvas();
    smallMapCanvas_->setMargin(20, 20, 20, 20);
    smallMapCanvas_->w->set(350);
    smallMapCanvas_->h->set(300);
    smallMapCanvas_->anchor->bind(canvas_->anchor);
    smallMapCanvas_->isBoundsVisible->set(true);
    smallMapCanvas_->opacity->set(0.5);

    smallMapCamera_ = new Camera();
    smallMapCamera_->boundsRect->set(Rect(0, 0, 20000, 20000));
    smallMapCamera_->areaRect->set(Rect(0, 0, 4000, 3000));
    smallMapCamera_->location->bind(myGameObject_->body->location);
    smallMapCanvas_->setCamera(smallMapCamera_);

    smallMapCanvas_->addDrawable(drawableMap);
    Plot* plot = new Plot();
    plot->location->bind(myGameObject_->body->location);
    plot->size->set(1.0);
    smallMapCanvas_->addDrawable(plot);

    canvas_->addComponent(smallMapCanvas_);

    // --- ANIMATIONS ---

    MyGameObject* obj = new MyGameObject();
    obj->body->location->set(Point(4000, 8300));

    //world_->add(obj->body);
    canvas_->addDrawable(obj->spriteContainer);

    //

    isInitialized_ = true;
}

Canvas *Game::getExternalCanvas() {
    return externalCanvas_;
}
