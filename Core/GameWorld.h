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

#ifndef __GameWorld_H_
#define __GameWorld_H_

#include <list>
#include "Map.h"
class Map;
#include "CollisionEventArgs.h"
#include "CollisionEventHandler.h"
#include "GameEntity.h"
class GameEntity;

class GameWorld {
private:
    std::list<GameEntity*> *gameEntities;
    Map* map;
    Vector gForce;
    double metersPerPixel;
    double airDensity;
    void detectCollision(GameEntity* entity, Point oldLocation, Point newLocation);
public:
    GameWorld(Vector gforce, double metersPerPixel, double airDensity);
    void step(double timeSeconds);
    void addEntity(GameEntity *gameEntity);
    void setMap(Map* map);
    Map* getMap();
    long getW();
    long getH();
    std::list<GameEntity*> *getGameEntities();
};

#endif //__GameWorld_H_
