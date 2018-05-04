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


#include "precompile.h"
#include "DebugBehaviour.h"
#include "Tile2D.h"
#include "Prefabs.h"
#include "EnemyAIBase.h"
#include "Input.h"
#include "LightSystem.h"
#include "TileMap.h"
#include "Window.h"

void DebugBehaviour::awake() {
    body_ = gameObject()->getComponent<Body>();
    sprite_ = gameObject()->getComponent<Sprite>();

    checkpointsUpdate_();
}

void DebugBehaviour::update() {
    auto& keyboard = Tile2D::input().keyboard();

    if (keyboard.keyPressed(SDL_SCANCODE_G)) {
        auto walkingEnemy = Prefabs::walker();
        walkingEnemy->transform().setPosition(transform()->getPosition());
        walkingEnemy->getComponent<EnemyAIBase>()->setTarget(transform());
    }
    if (keyboard.keyPressed(SDL_SCANCODE_H)) {
        auto crabKindOf = Prefabs::fish();
        crabKindOf->transform().setPosition(transform()->getPosition());
        crabKindOf->getComponent<EnemyAIBase>()->setTarget(transform());
    }
    if (keyboard.keyPressed(SDL_SCANCODE_J)) {
        auto fourwayCyclops = Prefabs::evilEye();
        fourwayCyclops->transform().setPosition(transform()->getPosition());
        fourwayCyclops->getComponent<EnemyAIBase>()->setTarget(transform());
    }
    if (keyboard.keyPressed(SDL_SCANCODE_K)) {
        auto twoHorn = Prefabs::twoHorn();
        twoHorn->transform().setPosition(transform()->getPosition());
        twoHorn->getComponent<EnemyAIBase>()->setTarget(transform());
    }
    if (keyboard.keyPressed(SDL_SCANCODE_L)) {
        auto boss = Prefabs::boss();
        boss->transform().setPosition(transform()->getPosition());
        boss->getComponent<EnemyAIBase>()->setTarget(transform());
    }
    if (keyboard.keyPressed(SDL_SCANCODE_RETURN)) {
        Tile2D::setIsDebugMode(!Tile2D::isDebugMode());
        if (Tile2D::isDebugMode()) {
            sprite_->setOpacity(0.5);
        } else {
            sprite_->setOpacity(1.0);
        }
    }
    if (keyboard.keyPressed(SDL_SCANCODE_P)) {
        std::cout << transform()->getPosition().x << "," << transform()->getPosition().y << "\n";
    }
    if (keyboard.keyPressed(SDL_SCANCODE_Q)) {
        moveToNextCheckpoint_();
    }
    if (keyboard.keyPressed(SDL_SCANCODE_R)) {
        Tile2D::tileMap().load("data/maps/map.bmp", "data/maps/tileset.json");
        checkpointsUpdate_();
    }
    if (keyboard.keyPressed(SDL_SCANCODE_A)) {
        Tile2D::lightSystem().setEnabled(!Tile2D::lightSystem().isEnabled());
    }
    if (keyboard.keyPressed(SDL_SCANCODE_S)) {
        Tile2D::lightSystem().setBlendedShadowsEnabled(!Tile2D::lightSystem().isBlendedShadowsEnabled());
    }
    if (keyboard.keyPressed(SDL_SCANCODE_D)) {
        Tile2D::lightSystem().setSoftShadowsEnabled(!Tile2D::lightSystem().isSoftShadowsEnabled());
    }
    if (keyboard.keyPressed(SDL_SCANCODE_V)) {
        Tile2D::window().setVsync(!Tile2D::window().getVsync());
    }
}

void DebugBehaviour::lateUpdate() {

}

void DebugBehaviour::checkpointsUpdate_() {
    checkpoints_.clear();
    auto w = Tile2D::tileMap().getW();
    auto h = Tile2D::tileMap().getH();
    for (auto y = 0u; y < h; ++y) {
        for (auto x = 0u; x < w; ++x) {
            auto tile = Tile2D::tileMap().getValue(x, y);
            if (tile != nullptr && tile->getName() == "checkpoint") {
                checkpoints_.push_back(Tile2D::tileMap().getWorldCoords(Veci(x, y)));
            }
        }
    }
    nextCheckpoint_ = 0;
    moveToNextCheckpoint_();
}

void DebugBehaviour::moveToNextCheckpoint_() {
    if (checkpoints_.empty()) {
        transform()->setPosition({500.0, 250.0});
    }
    else {
        transform()->setPosition(checkpoints_[nextCheckpoint_]);
        nextCheckpoint_++;
        if (nextCheckpoint_ >= checkpoints_.size()) {
            nextCheckpoint_ = 0;
        }
    }
}
