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



#include "Vec.h"
#include "Tile2D.h"
#include "Sprite.h"
#include "Button.h"
#include "Text.h"
#include "UIPrefabs.h"
#include "Resources.h"
#include "UIAutoPositioner.h"

GameObject* UIPrefabs::button(
        const Vecf& position,
        const char* string,
        const float width,
        IEventHandler<Button, Button::ButtonEventArgs>* handler,
        bool active
) {
    auto buttonObj = Tile2D::createGameObject();

    auto uiAutoPositioner = buttonObj->attachComponent<UIAutoPositioner>();
    uiAutoPositioner->setRelativePosition(position);

    Rect rect = {-width / 2.0f, -30.0f, width / 2.0f, 30.0f};

    static const Color textColorNormal      = {1.0f, 1.0f, 1.0f};
    static const Vecf scaleNormal           = {1.0f, 1.0f};

    static const Color textColorHighlighted = {0.0f, 1.0f, 0.0f};
    static const Vecf scaleHighlighted      = {1.2f, 1.2f};

    static const Color textColorPressed     = {0.5f, 1.0f, 1.0f};
    static const Vecf scalePressed          = {1.4f, 1.4f};

    auto buttonText = buttonObj->attachComponent<Text>();
    buttonText->setHorizontalAlignment(Text::HorizontalAlignment::center);
    buttonText->setVerticalAlignment(Text::VerticalAlignment::center);
    buttonText->setIsUIDrawable(true);
    buttonText->setString(string);
    buttonText->setFontPtr(Tile2D::resources().fonts["smallfont"]);
    buttonText->setFontSize(4.0f);
    buttonText->setColor(textColorNormal);

    static auto setNormalButton = [] (Button* button) {
        button->gameObject()->getComponent<Text>()->setColor(textColorNormal);
        button->gameObject()->transform().setScale(scaleNormal);
    };

    static auto setHighlightedButton = [] (Button* button) {
        button->gameObject()->getComponent<Text>()->setColor(textColorHighlighted);
        button->gameObject()->transform().setScale(scaleHighlighted);
    };

    static auto setPressedButton = [] (Button* button) {
        button->gameObject()->getComponent<Text>()->setColor(textColorPressed);
        button->gameObject()->transform().setScale(scalePressed);
    };

    auto buttonBehaviour = buttonObj->attachComponent<Button>();
    buttonBehaviour->setRect(rect);
    buttonBehaviour->clicked.add(handler);
    buttonBehaviour->mouseOver.add([] (Button* button, Button::ButtonEventArgs args) {
        if (!args.buttonIsPressed) {
            setHighlightedButton(button);
        } else {
            setPressedButton(button);
        }
    });
    buttonBehaviour->mouseOut.add([] (Button* button, Button::ButtonEventArgs args) {
        if (!args.buttonIsPressed) {
            setNormalButton(button);
        }
    });
    buttonBehaviour->pressed.add([] (Button* button, Button::ButtonEventArgs args) {
        setPressedButton(button);
    });
    buttonBehaviour->released.add([] (Button* button, Button::ButtonEventArgs args) {
        setNormalButton(button);
    });

    buttonObj->setIsActive(active);

    return buttonObj;
}


GameObject* UIPrefabs::text(
        const Vecf& position,
        const char* string,
        float size,
        Text::HorizontalAlignment horizontalAlignment,
        Text::VerticalAlignment verticalAlignment,
        bool active
) {
    auto textObj = Tile2D::createGameObject();

    auto uiAutoPositioner = textObj->attachComponent<UIAutoPositioner>();
    uiAutoPositioner->setRelativePosition(position);

    auto text = textObj->attachComponent<Text>();
    text->setHorizontalAlignment(horizontalAlignment);
    text->setVerticalAlignment(verticalAlignment);
    text->setIsUIDrawable(true);
    text->setString(string);
    text->setFontPtr(Tile2D::resources().fonts["smallfont"]);
    text->setFontSize(size);

    textObj->setIsActive(active);

    return textObj;
}

GameObject *UIPrefabs::button(
        const Vecf &position,
        const char *string,
        const float width,
        void (*handler)(Button *, Button::ButtonEventArgs),
        bool active
) {
    return button(
            position,
            string,
            width,
            new EventHandlerWrapper<Button, Button::ButtonEventArgs>(handler),
            active
    );
}
