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
        clicked     (Event<Button, ButtonEventArgs>(this)),
        pressed     (Event<Button, ButtonEventArgs>(this)),
        released    (Event<Button, ButtonEventArgs>(this)),
        mouseOver   (Event<Button, ButtonEventArgs>(this)),
        mouseOut    (Event<Button, ButtonEventArgs>(this)),

        rect_({-1.0f, -1.0f, 1.0f, 1.0f}),

        mousePressedOnThis_(false),
        mouseOver_(false)
{

}

void Button::awake() { }

void Button::update() {
    auto& mouse = Tile2D::input().mouse();

    Veci mousePosition;

    const Vecf& scale = transform()->getScale();
    const Vecf& position = transform()->getPosition();
    Rect renderedRect = {
            rect_.x1 * scale.x + position.x,
            rect_.y1 * scale.y + position.y,
            rect_.x2 * scale.x + position.x,
            rect_.y2 * scale.y + position.y
    };

    if (mouse.buttonPressed(SDL_BUTTON_LEFT, mousePosition)) {
        if (renderedRect.hasPointInside({(float)mousePosition.x, (float)mousePosition.y})) {
            mousePressedOnThis_ = true;
            pressed.raise({ mousePressedOnThis_ });
        }
    }

    if (mouse.buttonReleased(SDL_BUTTON_LEFT, mousePosition)) {
        if (
                renderedRect.hasPointInside({(float)mousePosition.x, (float)mousePosition.y}) &&
                mousePressedOnThis_
        ) {
            clicked.raise({ mousePressedOnThis_ });
        }
        if (mousePressedOnThis_) {
            released.raise({ mousePressedOnThis_ });
        }
        mousePressedOnThis_ = false;
    }

    mousePosition = mouse.getMousePosition();

    if (renderedRect.hasPointInside({(float)mousePosition.x, (float)mousePosition.y})) {
        if (!mouseOver_) {
            mouseOver.raise({ mousePressedOnThis_ });
        }
        mouseOver_ = true;
    }
    else if (mouseOver_) {
        mouseOut.raise({ mousePressedOnThis_ });
        mouseOver_ = false;
    }
}

void Button::lateUpdate() {

}

const Rect &Button::getRect() const {
    return rect_;
}

void Button::setRect(const Rect &rect) {
    rect_ = rect;
}
