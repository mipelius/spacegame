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


#ifndef __UIPrefabs_H
#define __UIPrefabs_H

#include "GameObject.h"
#include "Button.h"
#include "Vec.h"
#include "Text.h"

class UIPrefabs {
public:
    static GameObject* button(
            const Vecf &position,
            const char *string,
            const float width,
            void (*handler) (Button* button, Button::ButtonClickedEventArgs args)
    );

    static GameObject* text(
            const Vecf& position,
            const char* string,
            float size,
            Text::HorizontalAlignment horizontalAlignment = Text::HorizontalAlignment::center,
            Text::VerticalAlignment verticalAlignment = Text::VerticalAlignment::center
    );

};


#endif //__UIPrefabs_H
