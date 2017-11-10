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

void SceneManager::loadScene(std::string sceneStr) {
    std::cout << "Scene " << sceneStr << " loaded.\n";

    // ------> just a placeholder -> this is going to be modified soon!

    App::initialize();

    Game* game = Game::getInstance();
    game->initialize();

    try {
        game->launch();
        delete game;
    }
    catch (std::exception& e) {
        std::cout << "Something went wrong!";
    }

    App::free();

   //  <-----
}
