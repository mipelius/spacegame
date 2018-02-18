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


#include "Button.h"
#include "Tile2D.h"
#include "Input.h"

Button::Button() :
        click(Event<Button, ButtonClickedEventArgs>(this)),
        normalColor_({1.0f, 1.0f, 1.0f}),
        highLightedColor_({0.7f, 0.7f, 0.9f}),
        pressedColor_({0.4f, 0.4f, 7.0f}),
        mousePressedOnThis_(false)
{

}

void Button::awake() {
    sprite_ = gameObject()->getComponent<Sprite>();
    if (sprite_ == nullptr) {
        throw std::runtime_error("Button can't be attached to game object without sprite!");
    }
    sprite_->setColor(normalColor_);
}

void Button::update() {
    auto& mouse = Tile2D::input().mouse();

    Veci mousePosition;

    const Rect& origRect = sprite_->getRect();
    const Vecf& scale = transform()->getScale();
    const Vecf& position = transform()->getPosition();
    Rect renderedRect = {
            origRect.x1 * scale.x + position.x,
            origRect.y1 * scale.y + position.y,
            origRect.x2 * scale.x + position.x,
            origRect.y2 * scale.y + position.y
    };

    if (mouse.buttonPressed(SDL_BUTTON_LEFT, mousePosition)) {
        if (renderedRect.hasPointInside({(float)mousePosition.x, (float)mousePosition.y})) {
            mousePressedOnThis_ = true;
        }
    }

    if (mouse.buttonReleased(SDL_BUTTON_LEFT, mousePosition)) {
        if (
                renderedRect.hasPointInside({(float)mousePosition.x, (float)mousePosition.y}) &&
                mousePressedOnThis_
        ) {
            click.raise({});
        }
        mousePressedOnThis_ = false;
    }

    mousePosition = mouse.getMousePosition();

    if (renderedRect.hasPointInside({(float)mousePosition.x, (float)mousePosition.y})) {
        if (mousePressedOnThis_) {
            sprite_->setColor(pressedColor_);
        } else {
            sprite_->setColor(highLightedColor_);
        }
    }
    else {
        sprite_->setColor(normalColor_);
    }
}

void Button::lateUpdate() {

}

const Color &Button::getNormalColor() const {
    return normalColor_;
}

void Button::setNormalColor(const Color &normalColor) {
    normalColor_ = normalColor;
}

const Color &Button::getHighLightedColor() const {
    return highLightedColor_;
}

void Button::setHighLightedColor(const Color &highLightedColor) {
    highLightedColor_ = highLightedColor;
}

const Color &Button::getPressedColor() const {
    return pressedColor_;
}

void Button::setPressedColor(const Color &pressedColor) {
    pressedColor_ = pressedColor;
}
