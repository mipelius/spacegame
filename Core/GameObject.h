// This file is part of SpaceGame.
// Copyright (C) 2014  Miika Pelkonen
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

#ifndef __GameObject_H_
#define __GameObject_H_

#include "GameEntity.h"
#include "CollisionShape.h"
#include "Texture.h"

class GameObject : public GameEntity {
private:
    Texture* texture;
    double mass;
    int w, h;
    const double ZERO_MASS = 0.0001;

public:
    GameObject(
            Point focus,
            Point location,
            Texture *texture,
            double mass,
            int w,
            int h,
            CollisionShape *collisionShape = nullptr
    );
    void render(int x, int y);
    double getMass();
};


#endif //__GameObject_H_
