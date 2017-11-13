//
// Created by Miika Pelkonen on 10/11/2017.
//

#include <map>
#include "Tile2D.h"

bool Tile2D::isLoaded_ = false;

Tile2D::Tile2D() {
    sceneManager_ = new SceneManager();
}

Tile2D::~Tile2D() {
    delete sceneManager_;
}

SceneManager &Tile2D::sceneManager() {
    return *instance_().sceneManager_;
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

    // TODO config!
    // TODO scenes!
    // TODO resources!
}
