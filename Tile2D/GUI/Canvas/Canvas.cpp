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
#include "App.h"
#include "Window.h"
#include "IDrawable.h"
#include "Camera.h"
#include "IShadowMask.h"

Canvas::Canvas() {
    camera_ = nullptr;
}

void Canvas::renderActual() {
    if (camera_ == nullptr) return;

    for (auto& shadowMask : shadowMasks_) {
        shadowMask->update(this);
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(
            camera_->areaRect.get().x1,
            camera_->areaRect.get().x2,
            camera_->areaRect.get().y2,
            camera_->areaRect.get().y1,
            -1.0,
            1.0
    );

    glMatrixMode(GL_MODELVIEW);

    for (auto& drawable : drawables_) {
        drawable->draw(this);
    }

    for (auto& shadowMask : shadowMasks_) {
        shadowMask->draw(this);
    }
}

void Canvas::addDrawable(IDrawable *drawable) {
    drawables_.push_back(drawable);
}

void Canvas::setCamera(Camera& camera) {
    camera_ = &camera;
}

Camera& Canvas::getCamera() {
    return *camera_;
}

void Canvas::addShadowMask(IShadowMask *shadowMask) {
    shadowMasks_.push_back(shadowMask);
}

void Canvas::removeDrawable(IDrawable *drawable) {
    drawables_.remove(drawable);
}
