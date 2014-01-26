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

#ifndef __WalkingCreature_H_
#define __WalkingCreature_H_

class EntityCollisionEventArgs;

#include "SpaceGameObject.h"

class WalkingCreature : public SpaceGameObject {
private:
    bool isAbleToMove;
    bool _isWalking;
    bool collidedInLastStepXAxis;
    GameEntity* target;
protected:
    void onMapCollision();
public:
    WalkingCreature(Point location, int maxHealth);
    void walkLeft();
    void walkRight();
    void jump();
    void disableAbilityToMove();
    void enableAbilityToMove();

    void setWalking();
    bool isWalking();
    void setNotWalking();
    void setCollisionXAxis();
    void beforeStep(double timeElapsedSeconds);
    void setTarget(GameEntity* target);

};


#endif //__WalkingCreature_H_