// This file is part of SpaceGame.
// Copyright (C) 2014 Miika Pelkonen
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

#ifndef __Missile_H_
#define __Missile_H_

class Body;
class SpriteContainer;
class AnimatedTexture;
class Sample;

template <typename T> class Property;

#include "Point.h"

class Missile {
    friend class MissileBody;
    friend class Body_MapCollisionEventHandler;

public:
    Missile(Point initialLocation, Vector force);
    ~Missile();

    Body* const body;
    SpriteContainer* const spriteContainer;

private:
    AnimatedTexture* accelerationAnimation_;

    class MissileBody;

    class Body_MapCollisionEventHandler;
    class Body_BodyCollisionEventHandler;

    static Sample* missileCollisionSample_;
    static int sampleChannel_;
};

#endif //__Missile_H_
