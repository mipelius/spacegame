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

#ifndef SPACEGAME_PREFABS_H
#define SPACEGAME_PREFABS_H

#include <string>
#include <vector>
#include "GameObject.h"
#include "Rect.h"
#include "Color.h"

class Prefabs {

public:
    // enemies

    static GameObject* boss();
    static GameObject* fourwayCyclops();
    static GameObject* crabKindOf();
    static GameObject* walkingEnemy();
    static GameObject* twoHorn();

    // player stuff to drop / shoot

    static GameObject* laser();
    static GameObject* bomb();
    static GameObject* light();

    // effects

    static GameObject* bloodBurst();
    static GameObject* explosion();
    static void sparkles(Vecf position, Vecf normal, Color color);
    static void sparkle(Vecf position, Vecf velocity, Color color);
    static void pulseLight(Vecf position);

private:
    static GameObject* spawnEnemy_(
            std::string textureName,
            std::vector<Vecf> colliderPoints,
            Rect spriteRect,
            float gravityFactor
    );
};

#endif //SPACEGAME_PREFABS_H
