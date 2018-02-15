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


#ifndef SPACEGAME_BUTTON_H
#define SPACEGAME_BUTTON_H

#include "Tile2DComponent.h"
#include "Sprite.h"
#include "Event.h"
#include "Tile2DBehaviour.h"

class Button : public Tile2DBehaviour {

public:
    struct ButtonClickedEventArgs {
        // fill in later if necessary
    };

    Button();
    ~Button() override = default;

    const Event<Button, ButtonClickedEventArgs> click;

    const Color &getNormalColor() const;
    void setNormalColor(const Color &normalColor);
    const Color &getHighLightedColor() const;
    void setHighLightedColor(const Color &highLightedColor);
    const Color &getPressedColor() const;
    void setPressedColor(const Color &pressedColor);

protected:
    void awake() override;
    void update() override;
    void lateUpdate() override;

private:
    bool mousePressedOnThis_;

    Color normalColor_;
    Color highLightedColor_;
    Color pressedColor_;

    Sprite* sprite_;
};


#endif //SPACEGAME_BUTTON_H
