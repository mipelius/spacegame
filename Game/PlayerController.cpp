// This file is part of SpaceGame.
// Copyright (C) 2017 Miika Pelkonen
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

#include "PlayerController.h"

void PlayerController::awake() {
    body = gameObject()->getComponent<Body>();
    sprite = gameObject()->getComponent<Sprite>();
}

void PlayerController::update() {
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    if (state[SDL_SCANCODE_UP]) {
        body->force.set(Vec::byAngle(body->angle.get(), moveForce));
    }
    if (state[SDL_SCANCODE_LEFT]) {
        body->angle.set(body->angle.get() - 5);
    }
    if (state[SDL_SCANCODE_RIGHT]) {
        body->angle.set(body->angle.get() + 5);
    }

}
