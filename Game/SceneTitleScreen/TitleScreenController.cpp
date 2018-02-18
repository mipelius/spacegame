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

#include "TitleScreenController.h"
#include "Scenes.h"
#include "Tile2D.h"
#include "Input.h"
#include "SceneManager.h"

void TitleScreenController::awake() {

}

void TitleScreenController::update() {
    auto& keyboard = Tile2D::input().keyboard();
    auto& mouse = Tile2D::input().mouse();

    if (keyboard.keyPressed(SDL_SCANCODE_RETURN)) {
        Tile2D::sceneManager().loadScene(Scenes::inGame);
    }
    if (keyboard.keyPressed(SDL_SCANCODE_ESCAPE)) {
        Tile2D::quit();
    }
}

void TitleScreenController::lateUpdate() {

}