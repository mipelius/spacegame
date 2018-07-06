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


#include "JsonFileManager.h"
#include "precompile.h"
#include "Canvas.h"
#include "Window.h"
#include "DrawableBase.h"
#include "Camera.h"
#include "Tile2D.h"
#include "LightSystem.h"
#include "TileMap.h"
#include "PhysicsWorld.h"
#include "GameObject.h"

Canvas::Canvas() {
    camera_ = nullptr;
}

void Canvas::render() {
    sortDrawables_();
    auto it = drawables_.begin();

    if (camera_ != nullptr) {
        Tile2D::lightSystem().update(Tile2D::canvas());
        setCameraProjection_();
        renderDrawablesBeforeTerrain_(it);
        if (Tile2D::tileMap().isLoaded()) {
            Tile2D::tileMap().drawableMap_->draw(*this);
        }
        renderDrawablesAfterTerrain_(it);
        Tile2D::lightSystem().draw(Tile2D::canvas());

        if (Tile2D::isDebugMode()) {
            Tile2D::physicsWorld().debugDraw();
        }
    }

    setUIProjection_();
    renderUIDrawables_(it);
}

void Canvas::addDrawable(DrawableBase *drawable) {
    drawables_.push_back(drawable);
}

void Canvas::setCamera(Camera* camera) {
    camera_ = camera;
}

Camera* Canvas::getCamera() const {
    return camera_;
}

void Canvas::removeDrawable(DrawableBase *drawable) {
    drawables_.remove(drawable);
}

void Canvas::setCameraProjection_() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(
            camera_->getAreaRect().x1,
            camera_->getAreaRect().x2,
            camera_->getAreaRect().y2,
            camera_->getAreaRect().y1,
            -1.0,
            1.0
    );
}


void Canvas::setUIProjection_() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    auto rect = Tile2D::window().getRect();
    glOrtho(
            rect.x1,
            rect.x2,
            rect.y2,
            rect.y1,
            -1.0,
            1.0
    );
}


void Canvas::sortDrawables_() {
    drawables_.sort([](DrawableBase* drawableA, DrawableBase* drawableB){
        if (drawableA->isUIDrawable_ != drawableB->isUIDrawable_) {
            return !drawableA->isUIDrawable_; // and drawableB->isUIDrawable_
        } else {
            return drawableA->sortingLayer_->order < drawableB->sortingLayer_->order;
        }
    });
}

void Canvas::renderDrawablesBeforeTerrain_(std::list<DrawableBase*>::iterator& it) {
    // render drawables with conditions sortingLayer < 0 AND isUIDrawable == false
    for (; it != drawables_.end(); it++) {
        if ((*it)->sortingLayer_ == 0 || (*it)->isUIDrawable_) {
            break;
        }
        renderDrawable_((*it));
    }
}

void Canvas::renderDrawablesAfterTerrain_(std::list<DrawableBase*>::iterator& it) {
    for (; it != drawables_.end(); it++) {
        if ((*it)->isUIDrawable_) {
            break;
        }
        renderDrawable_((*it));
    }
}

void Canvas::renderUIDrawables_(std::list<DrawableBase*>::iterator& it) {
    for (; it != drawables_.end(); it++) {
        renderDrawable_((*it));
    }
}

void Canvas::renderDrawable_(DrawableBase *drawable) {
    if (drawable->gameObject()->isActive()) {
        drawable->draw(*this);
    }
}

const std::map<int, SortingLayer> &Canvas::getSortingLayers() {
    return sortingLayerMap_;
}

void Canvas::init(const std::string &sortingLayersFile) {
    auto jsonObject = JsonFileManager::load(sortingLayersFile);

    auto sortingLayersJson = jsonObject["sortingLayers"].ToArray();

    for (auto sortingLayerJson : sortingLayersJson) {
        int id = sortingLayerJson["id"].ToInt();

        SortingLayer sortingLayer = {
                sortingLayerJson["id"].ToInt(),
                sortingLayerJson["name"].ToString(),
                sortingLayerJson["order"].ToInt()
        };
        sortingLayerMap_[id] = sortingLayer;
    }
}

const SortingLayer &Canvas::getSortingLayer(int id) const {
    auto it = sortingLayerMap_.find(id);

    if (it == sortingLayerMap_.end()) {
        throw std::runtime_error("Canvas: no collider layer for id " + std::to_string(id));
    }

    return (*it).second;
}
