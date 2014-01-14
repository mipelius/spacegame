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

#ifndef __CollisionEventArgs_H_
#define __CollisionEventArgs_H_

#include "EventArgs.h"
#include "Point.h"
#include "Map.h"
#include "GameEntity.h"
class GameEntity;

class CollisionEventArgs : public EventArgs {
public:
    CollisionEventArgs();
    Point oldLocation;
    Point newLocation;
    Point collisionLocation;
    Map* map;
    GameEntity* otherEntity;
};


#endif //__CollisionEventArgs_H_
