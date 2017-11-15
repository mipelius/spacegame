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
#include <Game/Game.h>
#include "SceneManager.h"

SceneManager::SceneManager() {
    std::cout << "SceneManager -> startUp\n";
}

SceneManager::~SceneManager() {
    std::cout << "SceneManager -> shutDown\n";
}

void SceneManager::loadScene(int scene) {
    scenes_[scene]->init();
}

void SceneManager::init(std::map<int, IScene *> &scenes) {
    scenes_ = scenes;
    loadScene(0);
}