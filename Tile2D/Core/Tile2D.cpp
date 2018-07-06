// MIT License
//
// This file is part of SpaceGame.
// Copyright (c) 2014-2018 Miika Pelkonen
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


#include <map>
#include <vector>
#include "ColliderLayerMatrix.h"
#include "JsonFileManager.h"
#include "Canvas.h"
#include "Tile2D.h"
#include "PathFinder.h"
#include "LightSystem.h"
#include "PhysicsWorld.h"
#include "SceneManager.h"
#include "Resources.h"
#include "Window.h"
#include "GameObject.h"
#include "Tile2DBehaviour.h"
#include "PathFinder.h"
#include "Input.h"
#include "t2Time.h"
#include "ObjectCreator.h"
#include "json.h"
#include "Reflector.h"

bool Tile2D::isLoaded_ = false;

Tile2D::Tile2D() :
        particlePool_(Pool<Particle>(MAX_PARTICLES)),
        quit_(false)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    reflector_ = new Reflector();
    window_ = new Window();
    resources_ = new Resources();
    physicsWorld_ = new PhysicsWorld();
    sceneManager_ = new SceneManager();
    canvas_ = new Canvas();
    tileMap_ = new TileMap();
    lightSystem_ = new LightSystem();
    pathFinder_ = new PathFinder();
    input_ = new Input();
    time_ = new Time();
}

Tile2D::~Tile2D() {
    cleanUp_();

    delete time_;
    delete input_;
    delete pathFinder_;
    delete lightSystem_;
    delete tileMap_;
    delete canvas_;
    delete sceneManager_;
    delete physicsWorld_;
    delete resources_;
    delete window_;
    delete reflector_;

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
        const std::string&                      configFile,
        const std::string&                      resourcesFile,
        const std::string&                      sortingLayersFile,
        const std::string&                      colliderLayersFile,
        const std::string&                      tagsFile,
        const std::string&                      scenesFile,
        std::map<std::string, IObjectCreator*>  classBindings
) {
    // LOAD

    if (isLoaded_) {
        std::string error = "Tile2D is already loaded!";
        throw std::runtime_error(error);
    }
    isLoaded_ = true;
    instance_();

    // INIT
    Tile2D::reflector().init_(std::move(classBindings));
    Tile2D::window().init(configFile);
    Tile2D::canvas().init(sortingLayersFile);
    Tile2D::instance_().initTags_(tagsFile);
    Tile2D::lightSystem().init();
    Tile2D::physicsWorld().init(colliderLayersFile);
    Tile2D::setIsDebugMode(false);
    Tile2D::resources().init(resourcesFile);
    Tile2D::sceneManager().init(scenesFile);

    // START LOOP

    Tile2D::instance_().mainLoop_();
}

void Tile2D::mainLoop_() {
    Uint32 timestamp = SDL_GetTicks();

    while(!SDL_QuitRequested() && !quit_) {
        glClear(GL_COLOR_BUFFER_BIT);

        Uint32 ticks = SDL_GetTicks();
        time_->deltaTime_ = ticks - timestamp;
        timestamp = ticks;

        sceneManager_->update_();
        initGameObjects_();
        removeDestroyedObjects_();
        input_->PollSDL_Events_();
        updateBehaviours_();
        executeDelayedFunctions_();
        physicsWorld_->step(time_->deltaTime_ / 1000.0f);
        lateUpdateBehaviours_();
        canvas().render();
        window().swap_();
    }
}

void Tile2D::cleanUp_() {
    for (auto obj : objects_) {
        obj->destroy();
    }
    removeDestroyedObjects_();
    objectsToInit_.clear();
    delayedFunctions_.clear();
}

void Tile2D::removeDestroyedObjects_() {
    for (auto obj : objects_) {
        if (!obj->isAlive_) {
            obj->prepareDestroy_();
        }
    }

    auto it = objects_.begin();
    while (it != objects_.end())
    {
        if ((*it)->canBeDestroyed_)
        {
            delete *it;
            objects_.erase(it++);
        }
        else
        {
            ++it;
        }
    }
}

void Tile2D::initGameObjects_() {
    for (auto& obj: objectsToInit_) {
        obj->initializeComponents_();
    }
    objectsToInit_.clear();
}

void Tile2D::updateBehaviours_() {
    for (auto obj: behaviours_) {
        if (obj->gameObject()->isActive()) {
            obj->update();
        }
    }
}

void Tile2D::lateUpdateBehaviours_() {
    for (auto obj: behaviours_) {
        if (obj->gameObject()->isActive()) {
            obj->lateUpdate();
        }
    }
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

PathFinder &Tile2D::pathFinder() {
    return *instance_().pathFinder_;
}

const Time &Tile2D::time() {
    return *instance_().time_;
}

GameObject *Tile2D::createGameObject() {
    auto gameObject = new GameObject();
    instance_().onGameObjectCreated_(gameObject);
    return gameObject;
}

GameObject *Tile2D::createGameObject(const json::Object& jsonObject) {
    auto gameObject = new GameObject(jsonObject);
    instance_().onGameObjectCreated_(gameObject);
    return gameObject;
}

std::string Tile2D::getResourcePath() {
    char *basePath = SDL_GetBasePath();

    std::string path;

    if (basePath) {
        path = basePath;
    } else {
        path = SDL_strdup("./");
    }

    SDL_free(basePath);

    return path;
}

bool Tile2D::isDebugMode() {
    return instance_().isDebugMode_;
}

void Tile2D::setIsDebugMode(bool isDebugMode) {
    instance_().isDebugMode_ = isDebugMode;
}

void Tile2D::quit() {
    instance_().quit_ = true;
}

const Input &Tile2D::input() {
    return *instance_().input_;
}

void Tile2D::executeDelayedFunction(
        GameObject *gameObject,
        Uint32 delay,
        void (*function)(GameObject *)
) {
    DelayedFunction delayedFunction = {gameObject, delay, function, SDL_GetTicks()};
    instance_().delayedFunctions_.push_back(delayedFunction);
}

void Tile2D::executeDelayedFunctions_() {
    auto ticks = SDL_GetTicks();
    for (auto it = delayedFunctions_.begin(); it != delayedFunctions_.end(); ) {
        auto deltaTime = ticks - (*it).timestamp;
        if (deltaTime > (*it).delay) {
            (*it).function((*it).gameObject);
            delayedFunctions_.erase(it++);
        } else {
            ++it;
        }
    }
}

bool Tile2D::isLoaded() {
    return isLoaded_;
}

void Tile2D::onGameObjectCreated_(GameObject *gameObject) {
    instance_().objectsToInit_.push_back(gameObject);
    instance_().objects_.insert(gameObject);
}

Reflector& Tile2D::reflector() {
    return *instance_().reflector_;
}

const Tag &Tile2D::getTag(int id) {
    auto it = instance_().tagMap_.find(id);

    if (it == instance_().tagMap_.end()) {
        throw std::runtime_error("Tile2D: no tag for id " + std::to_string(id));
    }

    return (*it).second;
}

void Tile2D::initTags_(std::string tagsFile) {
    auto jsonObject = JsonFileManager::load(tagsFile);

    auto tagsJson = jsonObject["tags"].ToArray();

    for (const auto& tagJson : tagsJson) {
        int         id      = tagJson["id"].ToInt();
        std::string name    = tagJson["name"].ToString();

        tagMap_[id] = { id, name };
    }
}
