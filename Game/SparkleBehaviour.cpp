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

#include <SDL_timer.h>
#include "SparkleBehaviour.h"

void SparkleBehaviour::awake() {
    awakeTimestamp = SDL_GetTicks();
    sprite = gameObject()->getComponent<Sprite>();
}

void SparkleBehaviour::update() {
    if (SDL_GetTicks() - awakeTimestamp > TTL) {
        sprite->opacity.set(0.0);
        gameObject()->destroy();
    } else {
        double opacity = 1.0 - (SDL_GetTicks() - awakeTimestamp) / (double)TTL;
        if (opacity < 0.0) {
            opacity = 0.0;
        }
        sprite->opacity.set(opacity);
    }
}
