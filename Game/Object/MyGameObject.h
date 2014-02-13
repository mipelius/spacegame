// TEMPORARY CLASS FOR TRYING OUT THE FEATURES OF THE TILE2D LIBRARY
//
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

#ifndef __MyGameObject_H_
#define __MyGameObject_H_

#include "Body.h"
#include "IEventHandler.h"
#include "SpriteContainer.h"

class MyGameObject {

public:
    MyGameObject();
    ~MyGameObject();

    Body* const body;
    SpriteContainer* const spriteContainer;

    Property<Point>* const location;

private:

    Point location_;

    class Body_MapCollisionEventHandler;
};

#endif //__MyGameObject_H_
