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



#include "Input.h"

bool Input::Mouse::buttonPressed(Uint8 SDL_mousebutton, Veci& position) const {
    for (auto& mouseButtonEvent : mouseButtonEvents_) {
        if (mouseButtonEvent.button == SDL_mousebutton && mouseButtonEvent.state == SDL_PRESSED) {
            position = {mouseButtonEvent.x, mouseButtonEvent.y};
            return true;
        }
    }
    return false;
}

bool Input::Mouse::buttonReleased(Uint8 SDL_mousebutton, Veci& position) const {
    for (auto& mouseButtonEvent : mouseButtonEvents_) {
        if (mouseButtonEvent.button == SDL_mousebutton && mouseButtonEvent.state == SDL_RELEASED) {
            position = {mouseButtonEvent.x, mouseButtonEvent.y};
            return true;
        }
    }
    return false;
}

Veci Input::Mouse::getMousePosition() const {
    Veci result;
    SDL_GetMouseState(&result.x, &result.y);
    return result;
}

bool Input::Keyboard::keyPressed(SDL_Scancode scancode) const {
    for (auto& keyboardEvent : keyboardEvents_) {
        if (
                keyboardEvent.keysym.scancode   == scancode &&
                keyboardEvent.type              == SDL_KEYDOWN &&
                keyboardEvent.repeat            == 0
        ) {
            return true;
        }
    }
    return false;
}

bool Input::Keyboard::keyReleased(SDL_Scancode scancode) const {
    for (auto& keyboardEvent : keyboardEvents_) {
        if (keyboardEvent.keysym.scancode == scancode && keyboardEvent.type == SDL_KEYUP) {
            return true;
        }
    }
    return false;
}

bool Input::Keyboard::keyState(SDL_Scancode scancode) const {
    return keyboardState_[scancode];
}

const Input::Mouse &Input::mouse() const {
    return mouse_;
}

const Input::Keyboard &Input::keyboard() const {
    return keyboard_;
}

void Input::PollSDL_Events_() {
    SDL_Event event;

    keyboard_.keyboardEvents_.clear();
    mouse_.mouseButtonEvents_.clear();

    while(SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
            keyboard_.keyboardEvents_.push_back(event.key);
        }
        if (event.type == SDL_MOUSEBUTTONUP || event.type == SDL_MOUSEBUTTONDOWN) {
            mouse_.mouseButtonEvents_.push_back(event.button);
        }
    }



    keyboard_.keyboardState_ = SDL_GetKeyboardState(0);
}