//
// Created by Miika Pelkonen on 10/11/2017.
//

#include "Tile2D.h"
#include "SceneManager.h"

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
    static Tile2D tile2D;
    return tile2D;
}
