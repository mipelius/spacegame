//
// Created by Miika Pelkonen on 10/11/2017.
//

#include <map>
#include "Tile2D.h"

bool Tile2D::isLoaded_ = false;

Tile2D::Tile2D() {
    sceneManager_ = new SceneManager();
    window_ = new Window();
}

Tile2D::~Tile2D() {
    delete sceneManager_;
    delete window_;
}

Tile2D &Tile2D::instance_() {
    if (!isLoaded_) {
        std::string error = "Tile2D is not loaded!";
        throw std::runtime_error(error);
    }
    static Tile2D tile2D;
    return tile2D;
}

void Tile2D::load(Config &config, Resources &resources, std::map<int, IScene*> scenes) {
    if (isLoaded_) {
        std::string error = "Tile2D is already loaded!";
        throw std::runtime_error(error);
    }
    isLoaded_ = true;
    instance_();

    Tile2D::window().init(
            config.window_x,
            config.window_y,
            config.window_w,
            config.window_h,
            config.window_enableFullScreen
    );

    Tile2D::sceneManager().scenes_ = std::move(scenes);
    Tile2D::sceneManager().loadScene(0);

    Tile2D::instance_().mainLoop_();
}

Window &Tile2D::window() {
    return *instance_().window_;
}

SceneManager &Tile2D::sceneManager() {
    return *instance_().sceneManager_;
}

void Tile2D::mainLoop_() {
    while(!SDL_QuitRequested()) {

    }
    SDL_Quit();
}
