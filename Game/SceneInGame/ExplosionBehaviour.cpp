// MIT License
//
// This file is part of SpaceGame.
// Copyright (c) 2014-2018 Miika Pelkonen
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

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
