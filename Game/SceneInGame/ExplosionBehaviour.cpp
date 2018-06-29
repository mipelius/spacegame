//
// Created by Miika Pelkonen on 29/06/2018.
//

#include "Tile2D.h"
#include "TileMap.h"
#include "ExplosionBehaviour.h"

void ExplosionBehaviour::deserialize(const json::Object &jsonObject) {
    if (jsonObject.HasKey("radius")) {
        radius_ = jsonObject["radius"].ToInt();
    }
}

Tile2DComponent *ExplosionBehaviour::clone() {
    return new ExplosionBehaviour(*this);
}

void ExplosionBehaviour::awake() {
    auto tileW = Tile2D::tileMap().getTileSet()->getTileW();
    auto tileH = Tile2D::tileMap().getTileSet()->getTileH();

    for (auto x=-radius_; x<radius_; ++x) {
        for (auto y=-radius_; y<radius_; ++y) {
            if (Vecf((float)x, (float)y).length() > radius_) {
                continue;
            }
            Vecf offset = {
                    (float)(x * tileW),
                    (float)(y * tileH)
            };
            Tile2D::tileMap().setValueScaled(
                    transform()->getPosition() + offset,
                    Tile2D::tileMap().getTileSet()->getEmptyBlock()
            );
        }
    }
}

void ExplosionBehaviour::update() { }

void ExplosionBehaviour::lateUpdate() { }

int ExplosionBehaviour::getRadius_() const {
    return radius_;
}

void ExplosionBehaviour::setRadius_(int radius_) {
    ExplosionBehaviour::radius_ = radius_;
}
