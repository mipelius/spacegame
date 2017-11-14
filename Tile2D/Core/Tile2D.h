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
#include "PhysicsWorld.h"
#include "SceneManager.h"
#include "Resources.h"
#include "Window.h"

class Tile2D {

public:
    Tile2D(Tile2D const &) = delete;
    void operator=(Tile2D const &)  = delete;

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

private:
    Window *window_;
    Resources *resources_;
    SceneManager *sceneManager_;
    PhysicsWorld *physicsWorld_;

    void mainLoop_();

    Tile2D();
    ~Tile2D();

    static Tile2D &instance_();
    static bool isLoaded_;
};

#endif //__TILE2D_H
