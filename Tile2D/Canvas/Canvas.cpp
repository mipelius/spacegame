// This file is part of SpaceGame.
// Copyright (C) 2014 Miika Pelkonen
//
// SpaceGame is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// SpaceGame is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SpaceGame.  If not, see <http://www.gnu.org/licenses/>.

#include "precompile.h"
#include "Canvas.h"
#include "Window.h"
#include "DrawableBase.h"
#include "Camera.h"
#include "Tile2D.h"

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
            return drawableA->sortingLayer_ < drawableB->sortingLayer_;
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
