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


#ifndef __TILE2D_H
#define __TILE2D_H

#include <map>
#include <assert.h>
#include <set>
#include "precompile.h"
#include "Particle.h"
#include "Pool.h"

class IObjectCreator;
class PathFinder;
class Window;
class SceneManager;
class TileMap;
class PhysicsWorld;
class Input;
class Tile2DBehaviour;
class Canvas;
class LightSystem;
class Resources;
class SceneBase;
class GameObject;
class Time;
class Reflector;

namespace json {
    class Object;
}

struct Tag {
    int         id;
    std::string name;
};

class Tile2D {
    friend class Tile2DObject;
    friend class GameObject;
    friend class Tile2DBehaviour;
    friend class SceneManager;
    friend class ParticleSystem;
    friend class Particle;

public:
    Tile2D(Tile2D const &)              = delete;
    Tile2D& operator=(Tile2D const &)   = delete;
    Tile2D(Tile2D &&)                   = delete;
    Tile2D& operator=(Tile2D &&)        = delete;

    void static load(
            const std::string&                          configFile,
            const std::string&                          resourcesFile,
            const std::string&                          sortingLayersFile,
            const std::string&                          colliderLayersFile,
            const std::string&                          tagsFile,
            const std::string&                          scenesFile,
            std::map<std::string, IObjectCreator*>      classBindings
    );

    static std::string getResourcePath();

    static Window &window();
    static Resources &resources();
    static SceneManager &sceneManager();
    static PhysicsWorld &physicsWorld();
    static Canvas &canvas();
    static LightSystem &lightSystem();
    static TileMap &tileMap();
    static PathFinder &pathFinder();
    static Reflector &reflector();

    static const Input &input();
    static const Time &time();

    static void executeDelayedFunction(
            GameObject* gameObject,
            Uint32      delay,
            void        (*function)(GameObject*)
    );

    static GameObject* createGameObject();
    static GameObject* createGameObject(const json::Object& jsonObject);

    static bool isDebugMode();
    static void setIsDebugMode(bool isDebugMode);
    static bool isLoaded();
    static void quit();

    static const Tag& getTag(int id);

private:
    Reflector* reflector_;
    Window *window_;
    Resources *resources_;
    SceneManager *sceneManager_;
    PhysicsWorld *physicsWorld_;
    Canvas* canvas_;
    LightSystem* lightSystem_;
    TileMap* tileMap_;
    PathFinder* pathFinder_;
    Input* input_;
    Time* time_;

    void mainLoop_();
    void cleanUp_();
    void removeDestroyedObjects_();
    void initGameObjects_();
    void updateBehaviours_();
    void lateUpdateBehaviours_();

    Tile2D();
    ~Tile2D();

    static Tile2D &instance_();

    static bool isLoaded_;

    std::set<GameObject*> objects_;
    std::list<GameObject*> objectsToInit_;
    std::list<GameObject*> objectsToDestroy_;

    struct DelayedFunction {
        GameObject* gameObject;
        Uint32 delay;
        void (*function)(GameObject*);
        Uint32 timestamp;
    };

    std::list<Tile2DBehaviour*> behaviours_;
    std::list<DelayedFunction> delayedFunctions_;

    void executeDelayedFunctions_();

    Pool<Particle> particlePool_;

    bool isDebugMode_;
    bool quit_;

    void onGameObjectCreated_(GameObject *gameObject);

    static const unsigned int MAX_PARTICLES = 10000;

    void initTags_(std::string tagsFile);

    std::map<int, Tag> tagMap_;
};

#endif //__TILE2D_H
