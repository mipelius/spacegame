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

#include <iostream>
#include "Tile2D.h"
#include "SceneTitleScreen.h"
#include "Scenes.h"
#include "Text.h"
#include "SortingLayers.h"
#include "TitleScreenController.h"
#include "Sprite.h"
#include "Button.h"

Button* createButton(const Vecf& position, const char* string, const float width) {
    auto buttonObj = Tile2D::createGameObject();
    buttonObj->transform().setPosition(position);
    auto buttonSprite = buttonObj->attachComponent<Sprite>();
    buttonSprite->setRect({-width / 2.0f, -30.0f, width / 2.0f, 30.0f});
    buttonSprite->setIsUIDrawable(true);
    buttonSprite->setSortingLayer(SortingLayers::UI_Button);
    auto buttonBehaviour = buttonObj->attachComponent<Button>();

    auto buttonText = buttonObj->attachComponent<Text>();
    buttonText->setHorizontalAlignment(Text::HorizontalAlignment::center);
    buttonText->setVerticalAlignment(Text::VerticalAlignment::center);
    buttonText->setIsUIDrawable(true);
    buttonText->setSortingLayer(SortingLayers::UI_ButtonText);
    buttonText->setString(string);
    buttonText->setFontPtr(Tile2D::resources().fonts["smallfont"]);
    buttonText->setFontSize(4.0f);
    buttonText->setColor({0.0f, 0.0f, 0.0f});

    return buttonBehaviour;
}

Text* createText(const Vecf& position, const char* string, float size) {
    auto textObj = Tile2D::createGameObject();
    auto text = textObj->attachComponent<Text>();
    text->transform()->setPosition(position);
    text->setHorizontalAlignment(Text::HorizontalAlignment::center);
    text->setVerticalAlignment(Text::VerticalAlignment::bottom);
    text->setIsUIDrawable(true);
    text->setSortingLayer(SortingLayers::UI);
    text->setString(string);
    text->setFontPtr(Tile2D::resources().fonts["smallfont"]);
    text->setFontSize(size);

    return text;
}

void SceneTitleScreen::init() {
    Vecf center = {Tile2D::window().getW() / 2.0f, Tile2D::window().getH() / 2.0f};

    auto welcomeText = createText(center + Vecf(0.0f, -300.0f), "Welcome to the", 4.0f);
    welcomeText->setHorizontalAlignment(Text::HorizontalAlignment::center);
    welcomeText->setVerticalAlignment(Text::VerticalAlignment::bottom);

    auto spaceGameText = createText(center + Vecf(0.0f, -300.0f), "Space Game", 10.0f);
    spaceGameText->setHorizontalAlignment(Text::HorizontalAlignment::center);
    spaceGameText->setVerticalAlignment(Text::VerticalAlignment::top);

    auto titleScreenController = Tile2D::createGameObject();
    titleScreenController->attachComponent<TitleScreenController>();

    auto buttonStart = createButton(center + Vecf(0.0f, -50.0f), "Start game", 400.0f);
    buttonStart->click.add([] (Button* button, Button::ButtonClickedEventArgs args){
        Tile2D::sceneManager().loadScene(Scenes::inGame);
    });
    auto buttonQuit = createButton(center + Vecf(0.0f, 50.0f), "Quit", 200.0f);
    buttonQuit->click.add([] (Button* button, Button::ButtonClickedEventArgs args){
        Tile2D::quit();
    });

}

void SceneTitleScreen::destroy() { }
