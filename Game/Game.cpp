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

#include "Map.h"
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
#include "LightMask.h"
#include "PointLight.h"

#include "Sprite.h"
#include "SpriteContainer.h"

#include <OpenGL/gl3ext.h>
#include <OpenGL/glext.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

Game::Game() {

    // --- WINDOW PROPERTIES ---

    int x = 0;
    int y = 0;
    int w = 1280;
    int h = 800;

    bool enableFullScreen = true;

    Window* window = App::getInstance()->getWindow();
    window->initialize(x, y, w, h, enableFullScreen);

    // --- CANVAS ---

    canvas_ = new Canvas();
    canvas_->setMargin(0, 0, 0, 0);
    canvas_->w->set(Canvas::SIZE_MAX_WIDTH);
    canvas_->h->set(Canvas::SIZE_MAX_HEIGHT);
    canvas_->anchor->set(Canvas::Anchor::TOP_RIGHT);

    Texture* texture = new Texture("images/bg1.jpg");
    Background* background = new Background();
    background->setRatio(0.5);
    background->setTexture(texture);

    canvas_->addDrawable(background);

    window->addComponent(canvas_);

    // --- WORLD & MAP ---

    map_ = new Map("images/map.bmp", 10, 10);
    world_ = new PhysicsWorld(Vector(0, 9.81), 0.20, 0.0001);
    world_->setMap(map_);

    DrawableMap* drawableMap = new DrawableMap();
    drawableMap->setMap(map_);

    std::string filenames[] = {
            "images/green_block.bmp",
            "images/red_block.bmp",
            "images/blue_block.bmp",
    };

    MapTexture *mapTexture = new MapTexture(10, 10, 3, filenames);
    drawableMap->setMapTexture(mapTexture);

    canvas_->addDrawable(drawableMap);

    camera_ = new Camera();
    camera_->boundsRect->set(Rect(0, 0, 20000, 20000));
    camera_->areaRect->set(Rect(0, 0, w, h));

    canvas_->setCamera(camera_);

    // --- PLAYER ---

    myGameObject_ = new MyGameObject();
    myGameObject_->body->location->set(Point(4000, 8000));
    myGameObject_->body->location->bind(myGameObject_->location);

    world_->add(myGameObject_->body);
    canvas_->addDrawable(myGameObject_->spriteContainer);

    camera_->location->bind(myGameObject_->body->location);

    // --- LIGHT MASK  ---

    lightMask_ = new LightMask(w, h);
    canvas_->addLightMask(lightMask_);
    lightMask_->ambientLight->set(0.1);

    PointLight* light = new PointLight(Point(4000, 8000), 300);
    light->location->bind(myGameObject_->body->location);
    lightMask_->add(light);

    // --- SMALL MAP ---

    smallMapCanvas_ = new Canvas();
    smallMapCanvas_->setMargin(20, 20, 20, 20);
    smallMapCanvas_->w->set(350);
    smallMapCanvas_->h->set(300);
    smallMapCanvas_->anchor->bind(canvas_->anchor);
    smallMapCanvas_->isBoundsVisible->set(true);
    smallMapCanvas_->opacity->set(0.4);

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
}

void Game::launch() {
    App::getInstance()->getMusicPlayer()->play(new Music("music/spacegame.mp3"));

    const Uint8* keys;
    Uint32 timeMilliSec = 0;

    Texture* lightTexture = new Texture("images/light.png");

    PointLight* light1 = new PointLight(Point(5000, 9500), 250);
    lightMask_->add(light1);

    Sprite* lightSprite1 = new Sprite(lightTexture, Rect(-8, -8, 8, 8));
    SpriteContainer* lightSpriteContainer1 = new SpriteContainer();
    lightSpriteContainer1->addSprite(lightSprite1);
    canvas_->addDrawable(lightSpriteContainer1);

    lightSpriteContainer1->location->bind(light1->location);

    PointLight* light2 = new PointLight(Point(5000, 9500), 250);
    lightMask_->add(light2);

    Sprite* lightSprite2 = new Sprite(lightTexture, Rect(-8, -8, 8, 8));
    SpriteContainer* lightSpriteContainer2 = new SpriteContainer();
    lightSpriteContainer2->addSprite(lightSprite2);
    canvas_->addDrawable(lightSpriteContainer2);

    lightSpriteContainer2->location->bind(light2->location);

    double i = 0;

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
            myGameObject_->location->set(Point(4000, 8000));
        }

        if (keys[SDL_SCANCODE_RETURN]) {
            smallMapCanvas_->isVisible->toggle();
        }

        /// --- PHYSICS ---

        Uint32 timeElapsedMilliSec = SDL_GetTicks() - timeMilliSec;
        timeMilliSec = SDL_GetTicks();
        world_->step(timeElapsedMilliSec / 1000.0);

        /// --- ANIMATION ---

        i += 2.0;

        Point point = myGameObject_->location->get();

        light1->location->set(point + Vector::byAngle(i, 150));
        light2->location->set(point + Vector::byAngle(i + 180.0, 150));

        App::getInstance()->getAnimationManager()->update(timeElapsedMilliSec / 1000.0);

        /// --- RENDERING ---

        App::getInstance()->getWindow()->update();
    }
}

Game::~Game() {
    delete map_;
    delete world_;
}
