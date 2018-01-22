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
bool Tile2D::isDebugMode = false;

Tile2D::Tile2D() {
    SDL_Init(SDL_INIT_EVERYTHING);
    window_ = new Window();
    resources_ = new Resources();
    physicsWorld_ = new PhysicsWorld();
    sceneManager_ = new SceneManager();
    canvas_ = new Canvas(); window_->addComponent(canvas_);
    tileMap_ = new TileMap();
    lightSystem_ = new LightSystem();
}

Tile2D::~Tile2D() {
    delete canvas_;
    delete sceneManager_;
    delete physicsWorld_;
    delete resources_;
    delete window_;
    delete lightSystem_;

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
        std::map<unsigned, IScene*> scenes
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
    Tile2D::lightSystem().init();

    // START LOOP

    Tile2D::instance_().mainLoop_();
}

void Tile2D::mainLoop_() {
    Uint32 timestamp = SDL_GetTicks();

    while(!SDL_QuitRequested()) {
        glClear(GL_COLOR_BUFFER_BIT);
        Uint32 deltaTime = SDL_GetTicks() - timestamp;
        timestamp = SDL_GetTicks();
        sceneManager_->update_();
        initGameObjects_();
        removeDestroyedObjects_();
        updateBehaviours_();
        physicsWorld_->step(deltaTime / 1000.0);
        lateUpdateBehaviours_();
        window().update_();
    }
}

void Tile2D::cleanUp_() {
    for (auto& obj : objects_) {
        delete obj;
    }
    objects_.clear();
}

void Tile2D::removeDestroyedObjects_() {
    for (auto obj : objectsToDestroy_) {
        delete obj;
    }
    objectsToDestroy_.clear();
}

void Tile2D::initGameObjects_() {
    for (auto& obj: objectsToInit_) {
        obj->initializeComponents_();
    }
    objectsToInit_.clear();
}

void Tile2D::updateBehaviours_() {
    for (auto obj: behaviours_) {
        obj->update();
    }
}

void Tile2D::lateUpdateBehaviours_() {
    for (auto obj: behaviours_) {
        obj->lateUpdate();
    }
}

void Tile2D::destroy_(GameObject* obj) {
    auto it = instance_().objects_.find(obj);

    if ((*it) != nullptr) {
        instance_().objects_.erase((*it));
    }

    instance_().objectsToDestroy_.insert(obj);
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

LightSystem &Tile2D::lightSystem() {
    return *instance_().lightSystem_;
};

TileMap &Tile2D::tileMap() {
    return *instance_().tileMap_;
}

GameObject *Tile2D::createGameObject() {
    auto gameObject = new GameObject();
    instance_().objects_.insert(gameObject);
    return gameObject;
}
