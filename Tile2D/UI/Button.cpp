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



#include "Button.h"
#include "Tile2D.h"
#include "Input.h"
#include "GameObject.h"

Button::Button() :
        clicked     (Event<Button, ButtonEventArgs>()),
        pressed     (Event<Button, ButtonEventArgs>()),
        released    (Event<Button, ButtonEventArgs>()),
        mouseOver   (Event<Button, ButtonEventArgs>()),
        mouseOut    (Event<Button, ButtonEventArgs>()),

        rect_({-1.0f, -1.0f, 1.0f, 1.0f}),

        mousePressedOnThis_(false),
        mouseOver_(false)
{

}

void Button::awake() { }

void Button::update() {
    if (!gameObject()->isActive()) {
        return;
    }

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
            pressed.raise(this, { mousePressedOnThis_ });
        }
    }

    if (mouse.buttonReleased(SDL_BUTTON_LEFT, mousePosition)) {
        if (
                renderedRect.hasPointInside({(float)mousePosition.x, (float)mousePosition.y}) &&
                mousePressedOnThis_
        ) {
            clicked.raise(this, { mousePressedOnThis_ });
        }
        if (mousePressedOnThis_) {
            released.raise(this, { mousePressedOnThis_ });
        }
        mousePressedOnThis_ = false;
    }

    mousePosition = mouse.getMousePosition();

    if (renderedRect.hasPointInside({(float)mousePosition.x, (float)mousePosition.y})) {
        if (!mouseOver_) {
            mouseOver.raise(this, { mousePressedOnThis_ });
        }
        mouseOver_ = true;
    }
    else if (mouseOver_) {
        mouseOut.raise(this, { mousePressedOnThis_ });
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
