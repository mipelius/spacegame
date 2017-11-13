//
// Created by Miika Pelkonen on 10/11/2017.
//

#include <iostream>
#include <App.h>
#include <Game/Game.h>
#include "SceneManager.h"

SceneManager::SceneManager() {
    std::cout << "SceneManager -> startUp\n";
}

SceneManager::~SceneManager() {
    std::cout << "SceneManager -> shutDown\n";
}

void SceneManager::loadScene(int scene) {
    std::cout << "Scene " << scene << " loaded.\n";
}
