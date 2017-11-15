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

class Tile2D {
    friend class Tile2DObject;
public:
    Tile2D(Tile2D const &)              = delete;
    Tile2D& operator=(Tile2D const &)   = delete;
    Tile2D(Tile2D &&)                   = delete;
    Tile2D& operator=(Tile2D &&)        = delete;

    void static load(
            const std::string& configFile,
            std::vector<std::string> resTextures,
            std::vector<std::string> resSfx,
            std::vector<std::string> resMusic,
            std::vector<std::string> resFonts,
            std::vector<std::string> resBlockMappings,
            std::map<int, IScene*> scenes
    );

    static Window &window();
    static Resources &resources();
    static SceneManager &sceneManager();
    static PhysicsWorld &physicsWorld();

    template <class T> static T* create();

private:
    Window *window_;
    Resources *resources_;
    SceneManager *sceneManager_;
    PhysicsWorld *physicsWorld_;

    void mainLoop_();
    void cleanUp_();
    void removeDestroyedObjects_();

    Tile2D();
    ~Tile2D();

    static Tile2D &instance_();
    static bool isLoaded_;

    std::set<Tile2DObject*> objects_;
    std::list<Tile2DObject*> objectsToDestroy_;

    template <class T> static void destroy_(T* obj);
};

// template function definitions

template <class T>
T* Tile2D::create() {
    static_assert(std::is_base_of<Tile2DObject, T>(), "Can't create object since it is not inherited from Tile2DObject.");
    T* obj = new T();
    ((Tile2DObject*)obj)->createdByTile2D_ = true;
    instance_().objects_.insert(obj);
    return obj;
    // we could use more sophisticated memory allocator here... (e.g. dynamic object pool)
}

template <class T>
void Tile2D::destroy_(T* obj) {
    auto it = instance_().objects_.find(obj);
    assert((*it)->createdByTile2D_ && "Can't destroy object since it was not created by Tile2D.");
    if ((*it) != nullptr) {
        instance_().objects_.erase((*it));
    }
    instance_().objectsToDestroy_.push_back(obj);
}

#endif //__TILE2D_H
