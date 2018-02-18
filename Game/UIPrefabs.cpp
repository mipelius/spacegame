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


#include <Tile2D/Util/Primitives/Vec.h>
#include <Tile2D/Core/Tile2D.h>
#include <Tile2D/Canvas/Drawable/Sprite.h>
#include <Tile2D/UI/Button.h>
#include <Tile2D/Canvas/Drawable/Text.h>
#include "UIPrefabs.h"
#include "SortingLayers.h"
#include "Resources.h"

GameObject* UIPrefabs::button(
        const Vecf& position,
        const char* string,
        const float width,
        void (*handler) (Button* button, Button::ButtonClickedEventArgs args)
) {
    auto buttonObj = Tile2D::createGameObject();
    buttonObj->transform().setPosition(position);

    auto buttonSprite = buttonObj->attachComponent<Sprite>();
    buttonSprite->setRect({-width / 2.0f, -30.0f, width / 2.0f, 30.0f});
    buttonSprite->setIsUIDrawable(true);
    buttonSprite->setSortingLayer(SortingLayers::UI_Button);

    auto buttonBehaviour = buttonObj->attachComponent<Button>();
    buttonBehaviour->click.add(handler);

    auto buttonText = buttonObj->attachComponent<Text>();
    buttonText->setHorizontalAlignment(Text::HorizontalAlignment::center);
    buttonText->setVerticalAlignment(Text::VerticalAlignment::center);
    buttonText->setIsUIDrawable(true);
    buttonText->setSortingLayer(SortingLayers::UI_ButtonText);
    buttonText->setString(string);
    buttonText->setFontPtr(Tile2D::resources().fonts["smallfont"]);
    buttonText->setFontSize(4.0f);
    buttonText->setColor({0.0f, 0.0f, 0.0f});

    return buttonObj;
}


GameObject* UIPrefabs::text(
        const Vecf& position,
        const char* string,
        float size,
        Text::HorizontalAlignment horizontalAlignment,
        Text::VerticalAlignment verticalAlignment
) {
    auto textObj = Tile2D::createGameObject();
    auto text = textObj->attachComponent<Text>();
    text->transform()->setPosition(position);
    text->setHorizontalAlignment(horizontalAlignment);
    text->setVerticalAlignment(verticalAlignment);
    text->setIsUIDrawable(true);
    text->setSortingLayer(SortingLayers::UI);
    text->setString(string);
    text->setFontPtr(Tile2D::resources().fonts["smallfont"]);
    text->setFontSize(size);

    return textObj;
}