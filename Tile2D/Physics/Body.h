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

#ifndef __Body_H_
#define __Body_H_

#include "Point.h"
#include "Vector.h"
#include "IEventOwner.h"
#include "Property.h"

class Event;
class PhysicsWorld;
class CollisionShape;

class Body : public IEventOwner {
    friend class PhysicsWorld;

public:
    // properties

    Property<double>* const mass;

    Property<double>* const angle;
    Property<double>* const angularVelocity;
    Property<double>* const torque;

    Property<Point>* const location;
    Property<Vector>* const velocity;
    Property<Vector>* const speed;
    Property<Vector>* const force;

    // events

    Event* const bodyCollision;
    Event* const mapCollision;

    // methods

    Body(double mass);
    ~Body();

    void applyForce(Vector force);
    void applyTorque(double angle);

    PhysicsWorld* getWorld();
    CollisionShape* getCollisionShape();

    void setCollisionShape(CollisionShape* collisionShape);

    void ignoreCollisionDetection();
    void ignoreStep();

protected:
    PhysicsWorld * gameWorld_;
    CollisionShape* collisionShape_;

private:
    double mass_;

    double angle_;
    double angularVelocity_;
    double torque_;

    Point location_;
    Vector speed_;
    Vector velocity_;
    Vector force_;

    bool stepIsIgnored_;
    bool entityCollisionDetectionIsIgnored_;
    bool detectCollisionWith_(Body *otherBody);
    bool detectMapCollision_();
    void setWorld_(PhysicsWorld *gameWorld);
    void step_(double timeElapsedSec);
};

#endif //__Body_H_
