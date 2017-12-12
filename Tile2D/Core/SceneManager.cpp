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

#include <iostream>
#include "SceneManager.h"
#include "Tile2D.h"

SceneManager::SceneManager() {
    sceneToLoad_ = -1;
    currentScene_ = -1;
}

SceneManager::~SceneManager() {
    if (currentScene_ != -1) {
        scenes_[currentScene_]->destroy();
    }
    for (auto& scene : scenes_) {
        delete scene.second;
    }
}

void SceneManager::loadScene(unsigned scene) {
    sceneToLoad_ = scene;
}

void SceneManager::init(std::map<unsigned, IScene *> &scenes) {
    scenes_ = scenes;
    loadScene(0);
}

void SceneManager::update_() {
    if (sceneToLoad_ == -1) {
        return;
    }

    Tile2D::instance_().cleanUp_();

    if (currentScene_ != -1) {
        scenes_[currentScene_]->destroy();
    }
    currentScene_ = sceneToLoad_;
    int tmp = sceneToLoad_;
    sceneToLoad_ = -1;
    scenes_[tmp]->init();
}
