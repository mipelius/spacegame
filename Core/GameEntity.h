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



#ifndef __GameEntity_H_
#define __GameEntity_H_

#include "../Primitives/Point.h"
#include "../Primitives/Vector.h"
#include "../Event/Event.h"
#include "GameWorld.h"
#include "CollisionShape.h"

class Event;
class GameWorld;

class GameEntity {
protected:
    Point location;
    double angle;
    Point focus;
    Vector force;
    Vector speed;
    Vector velocity;
    Event* collisionEvent;
    GameWorld* gameWorld;
    bool _isDead;
    CollisionShape *collisionShape;
    GameEntity* owner;
public:
    GameEntity(Point focus, Point location, CollisionShape* collisionShape = nullptr);
    ~GameEntity();
    void die();
    void applyForce(Vector force);
    void setForceToZero();
    Vector getForce();
    void setAngle(double angle);
    double getAngle();
    void turnClockwise();
    void turnCounterClockwise();
    void setLocation(Point location);
    Point getLocation();
    void setFocus(Point focus);
    Point getFocus();
    void setSpeed(Vector speed);
    Vector getSpeed();
    Event* getCollisionEvent();
    void setWorld(GameWorld* gameWorld);
    GameWorld* getWorld();
    bool isDead();
    bool collidesWith(GameEntity *otherEntity);
    CollisionShape* getCollisionShape();
    void setOwner(GameEntity *owner);
    GameEntity* getOwner();
    void setCollisionShape(CollisionShape* collisionShape);
    Vector getVelocity();
    void setVelocity(Vector velocity);
    Point getLocationBeforeUpdate();
    virtual void render(double x, double y);
    virtual double getMass();

};


#endif //__GameEntity_H_
