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

#ifndef __Spaceship_H_
#define __Spaceship_H_

#include "SpaceGameObject.h"
#include <SDL2/SDL.h>

class Spaceship : public SpaceGameObject {
private:
    int health;
    void shootOnce(Point startPoint);
    Uint32 shootingDelay;
    Uint32 lastTimeShot;
    bool _isStuck;
    int size;
protected:
    void onEntityCollision(GameEntity* otherEntity);
    void onMapCollision();
public:
    Spaceship(Point location, int maxHealth, int size);
    void shoot();
    void forceShoot();
    void setShootingSpeed(Uint32 shootingPerSecond);
    void setStuck();
    void setNotStuck();
    bool isStuck();
};


#endif //__Spaceship_H_
