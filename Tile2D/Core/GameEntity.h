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

#include "Point.h"
#include "Vector.h"

class Event;
class GameWorld;
class CollisionShape;

class GameEntity {
    friend class GameWorld;
private:
    bool _isDead;
    bool _stepIsIgnored;
    bool _entityCollisionDetectionIsIgnored;
    bool _detectCollisionWith(GameEntity *otherEntity);
    bool _detectMapCollision();
    void _setWorld(GameWorld *gameWorld);
protected:
    Point location;
    double angle;
    double angularVelocity;
    double torque;
    Vector force;
    Vector speed;
    Vector velocity;
    Event* entityCollisionEvent;
    Event* mapCollisionEvent;
    GameWorld* gameWorld;
    CollisionShape *collisionShape;
    GameEntity* owner;

    virtual void step(double timeElapsedSec);

    virtual void beforeStep(double timeElapsedSec);
    virtual void afterStep(double timeElapsedSec);
    virtual void beforeEntityCollisionDetection(GameEntity* otherEntity);

    virtual void onMapCollision();
    virtual void onEntityCollision(GameEntity* otherEntity);
    virtual void onDying();
public:
    GameEntity(Point location, double angle, CollisionShape* collisionShape = nullptr);
    ~GameEntity();

    void die();
    bool isDead();

    void applyForce(Vector force);
    void setForceToZero();
    Vector getForce();

    void applyTorque(double angle);
    double getTorque();
    void setTorque(double torque);

    double getAngularVelocity();
    void setAngularVelocity(double angularVelocity);

    void setAngle(double angle);
    double getAngle();

    void setLocation(Point location);
    Point getLocation();

    void setSpeed(Vector speed);
    Vector getSpeed();

    Event* getEntityCollisionEvent();
    Event* getMapCollisionEvent();

    GameWorld* getWorld();

    CollisionShape* getCollisionShape();
    void setOwner(GameEntity *owner);
    GameEntity* getOwner();

    void setCollisionShape(CollisionShape* collisionShape);

    Vector getVelocity();
    void setVelocity(Vector velocity);

    Point getLocationBeforeUpdate();

    void ignoreEntityCollisionDetection();
    void ignoreStep();

    virtual void render(double x, double y);
    virtual double getMass();
};


#endif //__GameEntity_H_
