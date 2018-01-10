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

#ifndef __TILE2D_H
#define __TILE2D_H

#include <map>
#include <assert.h>
#include <set>
#include "PhysicsWorld.h"
#include "SceneManager.h"
#include "Resources.h"
#include "Window.h"
#include "Tile2DObject.h"
#include "Canvas.h"
#include "GameObject.h"
#include "Tile2DBehaviour.h"
#include "precompile.h"

class Tile2D {
    friend class Tile2DObject;
    friend class GameObject;
    friend class Tile2DBehaviour;
    friend class SceneManager;

public:
    static bool isDebugMode;

    Tile2D(Tile2D const &)              = delete;
    Tile2D& operator=(Tile2D const &)   = delete;
    Tile2D(Tile2D &&)                   = delete;
    Tile2D& operator=(Tile2D &&)        = delete;

    void static load(
            const std::string& configFile,
            const std::string& resourcesFile,
            std::map<unsigned, IScene*> scenes
    );

    static Window &window();
    static Resources &resources();
    static SceneManager &sceneManager();
    static PhysicsWorld &physicsWorld();
    static Canvas &canvas();

    template <class T> static T* create();

private:
    Window *window_;
    Resources *resources_;
    SceneManager *sceneManager_;
    PhysicsWorld *physicsWorld_;
    Canvas* canvas_;

    void mainLoop_();
    void cleanUp_();
    void removeDestroyedObjects_();
    void initGameObjects_();
    void updateBehaviours_();

    Tile2D();
    ~Tile2D();

    static Tile2D &instance_();

    static bool isLoaded_;

    std::set<void*> objects_;
    std::list<std::pair<void*, Tile2DObject*> > objectsToDestroy_;
    std::list<GameObject*> gameObjectsToInit_;
    std::list<Tile2DBehaviour*> behaviours_;

    void destroy_(Tile2DObject* obj);
};

// template function definition

template <class T>
T* Tile2D::create() {
    static_assert(std::is_base_of<Tile2DObject, T>(), "Can't create object since it is not inherited from Tile2DObject.");
    T* obj = new T();
    ((Tile2DObject*)obj)->derivedClassPtr = obj;
    auto result = instance_().objects_.insert(obj);
    obj->onCreate();
    return obj;
}


#endif //__TILE2D_H
