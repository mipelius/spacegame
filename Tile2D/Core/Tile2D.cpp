// This file is part of SpaceGame.
// Copyright (C) 2017 Miika Pelkonen
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

#include <map>
#include <vector>
#include "Canvas.h"
#include "Tile2D.h"

bool Tile2D::isLoaded_ = false;

Tile2D::Tile2D() {
    SDL_Init(SDL_INIT_EVERYTHING);
    window_ = new Window();
    resources_ = new Resources();
    physicsWorld_ = new PhysicsWorld();
    sceneManager_ = new SceneManager();
    canvas_ = new Canvas(); window_->addComponent(canvas_);
}

Tile2D::~Tile2D() {
    delete canvas_;
    delete sceneManager_;
    delete physicsWorld_;
    delete resources_;
    delete window_;

    cleanUp_();

    SDL_Quit();
}

Tile2D &Tile2D::instance_() {
    if (!isLoaded_) {
        std::string error = "Tile2D is not loaded!";
        throw std::runtime_error(error);
    }
    static Tile2D tile2D;
    return tile2D;
}

void Tile2D::load(
        const std::string& configFile,
        const std::string& resourcesFile,
        std::map<int, IScene*> scenes
) {
    // LOAD

    if (isLoaded_) {
        std::string error = "Tile2D is already loaded!";
        throw std::runtime_error(error);
    }
    isLoaded_ = true;
    instance_();

    // INIT

    Tile2D::window().init(configFile);
    Tile2D::resources().init(resourcesFile);
    Tile2D::sceneManager().init(scenes);

    // START LOOP

    Tile2D::instance_().mainLoop_();
}

void Tile2D::mainLoop_() {
    Uint32 timestamp = SDL_GetTicks();

    while(!SDL_QuitRequested()) {
        Uint32 deltaTime = SDL_GetTicks() - timestamp;
        timestamp = SDL_GetTicks();

        updateBehaviours_();
        physicsWorld_->step(deltaTime / 1000.0);
        window().update_();

        initGameObjects_();
        removeDestroyedObjects_();
    }
}

void Tile2D::cleanUp_() {
    for (auto& obj : objects_) {
        delete obj;
    }
    objects_.clear();
}

void Tile2D::removeDestroyedObjects_() {
    for (auto& obj : objectsToDestroy_) {
        delete obj;
    }
    objectsToDestroy_.clear();
}

void Tile2D::initGameObjects_() {
    for (auto& obj: gameObjectsToInit_) {
        obj->initializeComponents_();
    }
    gameObjectsToInit_.clear();
}

void Tile2D::updateBehaviours_() {
    for (auto& obj: behaviours_) {
        obj->update();
    }
}

void Tile2D::destroy_(Tile2DObject* obj) {
    void* actualObj = obj->derivedClassPtr;
    assert(actualObj != nullptr && "Can't destroy object since it was not created by Tile2D.");

    auto it = instance_().objects_.find(actualObj);

    if ((*it) != nullptr) {
        instance_().objects_.erase((*it));
    }

    instance_().objectsToDestroy_.push_back(actualObj);
}


Window &Tile2D::window() {
    return *instance_().window_;
}

Resources &Tile2D::resources() {
    return *instance_().resources_;
}

SceneManager &Tile2D::sceneManager() {
    return *instance_().sceneManager_;
}

PhysicsWorld &Tile2D::physicsWorld() {
    return *instance_().physicsWorld_;
}

Canvas &Tile2D::canvas() {
    return *instance_().canvas_;
}
