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

#include "Background.h"
#include "Resources.h"
#include "Tile2D.h"
#include "SceneTitleScreen.h"
#include "Scenes.h"
#include "Text.h"
#include "MovingCameraBehaviour.h"
#include "Button.h"
#include "UIPrefabs.h"
#include "Window.h"
#include "SceneManager.h"
#include "Camera.h"

void SceneTitleScreen::init() {
    // uncomment next line to load quick testing scene
    // Tile2D::sceneManager().loadScene(Scenes::quickTesting);

    Vecf center = {Tile2D::window().getW() / 2.0f, Tile2D::window().getH() / 2.0f};

    camera_ = new Camera();
    camera_->setAreaRect({0, 0, (float)Tile2D::window().getW(), (float)Tile2D::window().getH()});
    Tile2D::canvas().setCamera(camera_);

    auto background = Tile2D::createGameObject();
    auto bg = background->attachComponent<Background>();
    bg->setOpacity(0.5f);
    bg->setColor({0.5f, 1.0f, 1.0f});
    bg->setTexturePtr(Tile2D::resources().textures["bg1"]);

    auto titleScreenBehaviour = Tile2D::createGameObject();
    titleScreenBehaviour->attachComponent<MovingCameraBehaviour>();

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

void SceneTitleScreen::destroy() {
    delete camera_;
    Tile2D::canvas().setCamera(nullptr);
}
