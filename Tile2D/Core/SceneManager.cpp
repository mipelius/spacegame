//
// Created by Miika Pelkonen on 10/11/2017.
//

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
