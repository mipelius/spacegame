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
        if (keyboardEvent.keysym.scancode == scancode && keyboardEvent.state == SDL_PRESSED) {
            return true;
        }
    }
    return false;
}

bool Input::Keyboard::keyReleased(SDL_Scancode scancode) const {
    for (auto& keyboardEvent : keyboardEvents_) {
        if (keyboardEvent.keysym.scancode == scancode && keyboardEvent.state == SDL_RELEASED) {
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
