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


#include "Tile2D.h"
#include "TileMap.h"
#include "SceneQuickTesting.h"
#include "QuickTestingBehaviour.h"
#include "LightSystem.h"
#include "Camera.h"
#include "Window.h"

void SceneQuickTesting::init() {
    Tile2D::tileMap().load("data/maps/map.bmp", "data/maps/tileset.json");
    Tile2D::lightSystem().setEnabled(false);
    camera_ = new Camera();
    Tile2D::canvas().setCamera(camera_);
    camera_->setAreaRect({0, 0, (float)Tile2D::window().getW(), (float)Tile2D::window().getH()});

    GameObject* quickTestingObj = Tile2D::createGameObject();
    quickTestingObj->attachComponent<QuickTestingBehaviour>();
}

void SceneQuickTesting::destroy() {
    Tile2D::tileMap().unload();
    delete camera_;
}
