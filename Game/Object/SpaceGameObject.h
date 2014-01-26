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

#ifndef __SpaceGameEntity_H_
#define __SpaceGameEntity_H_

#include "GameObjectGroup.h"

class Controller;
class Team;

class SpaceGameObject : public GameObjectGroup {
    friend class Controller;
private:
    Controller *controller;
    int health;
    int maxHealth;
    Team* _team;
protected:
    void beforeEntityCollisionDetection(GameEntity *otherEntity);
    void onDying();
    virtual void beforeStep(double timeElapsedSec);
public:
    SpaceGameObject(Point location, double angle, CollisionShape* shape, int maxHealth);
    void damage(int damage);
    int getHealth();
    int getMaxHealth();
    void setTeam(Team* team);
    Team* getTeam();
};


#endif //__SpaceGameEntity_H_
