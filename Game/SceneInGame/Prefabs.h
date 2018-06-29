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


#ifndef SPACEGAME_PREFABS_H
#define SPACEGAME_PREFABS_H

#include <string>
#include <vector>
#include "GameObject.h"
#include "Rect.h"
#include "Color.h"
#include "Texture.h"
#include "PolygonCollider.h"

class Prefabs {

public:
    static GameObject *player();

    // effects
    static GameObject *light();

    // other
    static GameObject *hud(GameObject *player);

    static GameObject *enemySpawner(
            Rect area,
            GameObject *target,
            const std::string &prefabString,
            Uint32 spawningDelay
    );

    static GameObject *pickupSpawner(
            Rect area,
            GameObject *target,
            const std::string &prefabString,
            Uint32 spawningDelay,
            int itemTag,
            int maxItemCount,
            float minDistanceToTarget
    );
};

#endif //SPACEGAME_PREFABS_H
