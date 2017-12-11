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

SceneManager::SceneManager() {
    std::cout << "SceneManager -> startUp\n";
    currentScene_ = -1;
}

SceneManager::~SceneManager() {
    std::cout << "SceneManager -> shutDown\n";
    if (currentScene_ != -1) {
        scenes_[currentScene_]->destroy();
    }
    for (auto& scene : scenes_) {
        delete scene.second;
    }
}

void SceneManager::loadScene(unsigned scene) {
    if (currentScene_ != -1) {
        scenes_[currentScene_]->destroy();
    }
    currentScene_ = scene;
    scenes_[scene]->init();
}

void SceneManager::init(std::map<unsigned, IScene *> &scenes) {
    scenes_ = scenes;
    loadScene(0);
}
