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
#include "LightMask.h"
#include "ILight.h"

Canvas::Canvas() {
    camera_ = nullptr;
}

Canvas::~Canvas() { }


void Canvas::renderActual() {
    if (!camera_) return;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, camera_->areaRect->get().getWidth(), camera_->areaRect->get().getHeight(), 0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);

    for (std::list<IDrawable *>::iterator i = drawables_.begin(); i != drawables_.end(); i++) {
        (*i)->draw(this);
    }
}

void Canvas::addDrawable(IDrawable *drawable) {
    drawables_.push_back(drawable);
}

void Canvas::setCamera(Camera *camera) {
    camera_ = camera;
}

Camera *Canvas::getCamera() {
    return camera_;
}
