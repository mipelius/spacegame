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

#ifndef __SCENEMANAGER_H
#define __SCENEMANAGER_H

#include "IScene.h"
#include <map>

class SceneManager {
    friend class Tile2D;

public:
    void loadScene(unsigned scene);

private:
    SceneManager();
    ~SceneManager();

    void init(std::map<unsigned, IScene*>& scenes);

    int currentScene_;
    std::map<unsigned, IScene*> scenes_;
};


#endif //__SCENEMANAGER_H
