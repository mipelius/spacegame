// MIT License
//
// This file is part of SpaceGame.
// Copyright (c) 2014-2018 Miika Pelkonen
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


#include "Background.h"
#include "MovingCameraBehaviour.h"
#include "UIPrefabs.h"
#include "Tile2D.h"
#include "Scenes.h"
#include "SceneEndScreen.h"
#include "Window.h"
#include "SceneManager.h"
#include "Camera.h"
#include "Resources.h"

void SceneEndScreen::init() {
    Vecf center = {Tile2D::window().getW() / 2.0f, Tile2D::window().getH() / 2.0f};

    auto youWonText = UIPrefabs::text(
            center + Vecf(0.0f, -300.0f),
            "You won!",
            10.0f
    );

    auto buttonMainMenu = UIPrefabs::button(
            center + Vecf(0.0f, -50.0f),
            "Back to main menu",
            700.0f,
            [] (Button* button, Button::ButtonEventArgs args){
                Tile2D::sceneManager().loadScene(Scenes::titleScreen);
            }
    );

    camera_ = new Camera();
    camera_->setAreaRect({0, 0, (float)Tile2D::window().getW(), (float)Tile2D::window().getH()});
    Tile2D::canvas().setCamera(camera_);

    auto background = Tile2D::createGameObject();
    auto bg = background->attachComponent<Background>();
    bg->setOpacity(1.0f);
    bg->setColor({0.5f, 1.0f, 1.0f});
    bg->setTexturePtr(Tile2D::resources().textures["bg1"]);

    auto titleScreenBehaviour = Tile2D::createGameObject();
    titleScreenBehaviour->attachComponent<MovingCameraBehaviour>();

}

void SceneEndScreen::destroy() {
    delete camera_;
    Tile2D::canvas().setCamera(nullptr);
}
