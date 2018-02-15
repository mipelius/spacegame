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

void SceneTitleScreen::init() {
    // we are just loading the in game scene for now
    auto welcomeText = Tile2D::createGameObject();

    auto textWelcome = welcomeText->attachComponent<Text>();
    textWelcome->transform()->setPosition({Tile2D::window().getW() / 2.0f, Tile2D::window().getH() / 2.0f - 20.0f});
    textWelcome->setHorizontalAlignment(Text::HorizontalAlignment::center);
    textWelcome->setVerticalAlignment(Text::VerticalAlignment::bottom);
    textWelcome->setIsUIDrawable(true);
    textWelcome->setSortingLayer(SortingLayers::UI);
    textWelcome->setString("Welcome to the");
    textWelcome->setFontPtr(Tile2D::resources().fonts["smallfont"]);
    textWelcome->setFontSize(4.0f);

    auto spaceGameText = Tile2D::createGameObject();

    auto textSpaceGame = spaceGameText->attachComponent<Text>();
    textSpaceGame->transform()->setPosition({Tile2D::window().getW() / 2.0f, Tile2D::window().getH() / 2.0f - 20.0f});
    textSpaceGame->setHorizontalAlignment(Text::HorizontalAlignment::center);
    textSpaceGame->setVerticalAlignment(Text::VerticalAlignment::top);
    textSpaceGame->setIsUIDrawable(true);
    textSpaceGame->setSortingLayer(SortingLayers::UI);
    textSpaceGame->setString("Space Game");
    textSpaceGame->setFontPtr(Tile2D::resources().fonts["smallfont"]);
    textSpaceGame->setFontSize(10.0f);

    auto titleScreenController = Tile2D::createGameObject();
    titleScreenController->attachComponent<TitleScreenController>();
}

void SceneTitleScreen::destroy() { }
