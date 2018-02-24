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

#include "MovingCameraBehaviour.h"
#include "Tile2D.h"
#include "Camera.h"
#include "Canvas.h"
#include "t2Time.h"

void MovingCameraBehaviour::awake() {

}

void MovingCameraBehaviour::update() {
    Vecf move = {Tile2D::time().getDeltaTime() * 50.0f, 0.0f};
    auto cameraNewPos = Tile2D::canvas().getCamera()->getPosition() + move;
    Tile2D::canvas().getCamera()->setPosition(cameraNewPos);
}

void MovingCameraBehaviour::lateUpdate() {

}