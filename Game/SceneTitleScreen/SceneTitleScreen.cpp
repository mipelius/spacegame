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

#include "Tile2D.h"
#include "SceneTitleScreen.h"
#include "Scenes.h"
#include "Text.h"
#include "TitleScreenController.h"
#include "Button.h"
#include "UIPrefabs.h"
#include "Window.h"
#include "SceneManager.h"

void SceneTitleScreen::init() {
    // uncomment next line to load quick testing scene
    // Tile2D::sceneManager().loadScene(Scenes::quickTesting);

    Vecf center = {Tile2D::window().getW() / 2.0f, Tile2D::window().getH() / 2.0f};

    auto welcomeText = UIPrefabs::text(
            center + Vecf(0.0f, -300.0f),
            "Welcome to the",
            4.0f,
            Text::HorizontalAlignment::center,
            Text::VerticalAlignment::bottom
    );

    auto spaceGameText = UIPrefabs::text(
            center + Vecf(0.0f, -300.0f),
            "Space Game",
            10.0f,
            Text::HorizontalAlignment::center,
            Text::VerticalAlignment::top
    );

    auto titleScreenController = Tile2D::createGameObject();
    titleScreenController->attachComponent<TitleScreenController>();

    auto buttonStart = UIPrefabs::button(
            center + Vecf(0.0f, -50.0f),
            "Start game",
            400.0f,
            [] (Button* button, Button::ButtonEventArgs args) {
                Tile2D::sceneManager().loadScene(Scenes::inGame);
            }
    );
    auto buttonQuit = UIPrefabs::button(
            center + Vecf(0.0f, 50.0f),
            "Quit",
            200.0f,
            [] (Button* button, Button::ButtonEventArgs args) {
                Tile2D::quit();
            }
    );

}

void SceneTitleScreen::destroy() { }
