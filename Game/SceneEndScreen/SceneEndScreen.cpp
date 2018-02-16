// This file is part of SpaceGame.
// Copyright (C) 2017 Miika Pelkonen
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

#include <Game/UIPrefabs.h>
#include <Tile2D/Core/Tile2D.h>
#include <Game/Scenes.h>
#include "SceneEndScreen.h"

void SceneEndScreen::init() {
    Vecf center = {Tile2D::window().getW() / 2.0f, Tile2D::window().getH() / 2.0f};

    auto spaceGameText = UIPrefabs::text(
            center + Vecf(0.0f, -300.0f),
            "You won!",
            10.0f
    );

    auto buttonStart = UIPrefabs::button(
            center + Vecf(0.0f, -50.0f),
            "Back to main menu",
            700.0f,
            [] (Button* button, Button::ButtonClickedEventArgs args){
                Tile2D::sceneManager().loadScene(Scenes::titleScreen);
            }
    );
}

void SceneEndScreen::destroy() {

}
