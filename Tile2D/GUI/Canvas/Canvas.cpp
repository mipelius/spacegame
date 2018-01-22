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

Canvas::Canvas() {
    camera_ = nullptr;
}

void Canvas::renderActual() {
    if (camera_ == nullptr) return;

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

    for (auto& drawable : drawables_) {
        drawable->draw(*this);
    }
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
