// This file is part of SpaceGame.
// Copyright (C) 2018 Miika Pelkonen
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


#include "BackgroundBehaviour.h"
#include "Tile2D.h"
#include "Tile2DMath.h"

void BackgroundBehaviour::awake() {
    bg = gameObject()->getComponent<Background>();
}

void BackgroundBehaviour::update() {

}

void BackgroundBehaviour::lateUpdate() {
    Camera* camera = Tile2D::canvas().getCamera();

    float opacity = bg->getOpacity();

    if (area_.hasPointInside(camera->getPosition())) {
        opacity += fadeInOutSpeed / 60;
    } else {
        opacity -= fadeInOutSpeed / 60;
    }

    Mathf::clamp(opacity, 0.0f, 1.0f);

    bg->setOpacity(opacity);
}

void BackgroundBehaviour::setArea(const Rect &area) {
    BackgroundBehaviour::area_ = area;
}
