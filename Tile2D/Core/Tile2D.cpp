//
// Created by Miika Pelkonen on 10/11/2017.
//

#include <map>
#include <vector>
#include "Tile2D.h"

bool Tile2D::isLoaded_ = false;

Tile2D::Tile2D() {
    SDL_Init(SDL_INIT_EVERYTHING);
    window_ = new Window();
    resources_ = new Resources();
    sceneManager_ = new SceneManager();
}

Tile2D::~Tile2D() {
    delete sceneManager_;
    delete resources_;
    delete window_;

    SDL_Quit();
}

Tile2D &Tile2D::instance_() {
    if (!isLoaded_) {
        std::string error = "Tile2D is not loaded!";
        throw std::runtime_error(error);
    }
    static Tile2D tile2D;
    return tile2D;
}

void Tile2D::load(
        const std::string& configFile,
        std::vector<std::string> resTextures,
        std::vector<std::string> resSfx,
        std::vector<std::string> resMusic,
        std::vector<std::string> resFonts,
        std::vector<std::string> resBlockMappings,
        std::map<int, IScene*> scenes
) {
    // LOAD

    if (isLoaded_) {
        std::string error = "Tile2D is already loaded!";
        throw std::runtime_error(error);
    }
    isLoaded_ = true;
    instance_();

    // INIT

    Tile2D::window().init(configFile);
    Tile2D::resources().init(resTextures, resSfx, resMusic, resFonts, resBlockMappings);
    Tile2D::sceneManager().init(scenes);

    // START LOOP

    Tile2D::instance_().mainLoop_();
}

void Tile2D::mainLoop_() {
    while(!SDL_QuitRequested()) {

    }
}

Window &Tile2D::window() {
    return *instance_().window_;
}

Resources &Tile2D::resources() {
    return *instance_().resources_;
}

SceneManager &Tile2D::sceneManager() {
    return *instance_().sceneManager_;
}