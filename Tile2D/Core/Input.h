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


#ifndef SPACEGAME_INPUT_H
#define SPACEGAME_INPUT_H

#include <SDL2/SDL_scancode.h>
#include "Vec.h"

class Input {
    friend class Tile2D;
public:
    class Mouse {
        friend class Tile2D;
    public:
        bool buttonPressed(Uint8 SDL_mousebutton, Veci& position) const;
        bool buttonReleased(Uint8 SDL_mousebutton, Veci& position) const;

    private:
        std::list<SDL_MouseButtonEvent> mouseButtonEvents_;
    };

    class Keyboard {
        friend class Tile2D;
    public:
        bool keyPressed(SDL_Scancode scancode) const;
        bool keyReleased(SDL_Scancode scancode) const;
        bool keyState(SDL_Scancode scancode) const;
    private:
        const Uint8* keyboardState_;
        std::list<SDL_KeyboardEvent> keyboardEvents_;
    };

    const Mouse& mouse() const;
    const Keyboard& keyboard() const;

private:
    Input() = default;
    ~Input() = default;

    Mouse mouse_;
    Keyboard keyboard_;
};

#endif //SPACEGAME_INPUT_H
