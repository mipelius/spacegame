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

#include "Tile2DObject.h"
#include "Vec.h"
#include "Property.h"
#include "Event.h"

class PhysicsWorld;
class CollisionShape;
class BodyCollisionEventArgs;
class EventArgs;

class Body : public Tile2DObject {
    friend class PhysicsWorld;

public:
    // properties

    Property<double> const mass;

    Property<double> const angle;
    Property<double> const angularVelocity;
    Property<double> const torque;

    Property<Vec> const position;
    Property<Vec> const velocity;
    Property<Vec> const speed;
    Property<Vec> const force;

    // events

    Event<Body, BodyCollisionEventArgs> const bodyCollision;
    Event<Body, EventArgs> const mapCollision;

    // methods

    Body();

    void applyForce(Vec force);
    void applyTorque(double angle);

    PhysicsWorld* getWorld();
    CollisionShape& getCollisionShape();

    void setCollisionShape(CollisionShape* collisionShape);

    void ignoreCollisionDetection();
    void ignoreStep();

    virtual void afterStep();

    void die();

protected:
    PhysicsWorld *physicsWorld_;
    CollisionShape* collisionShape_;

    virtual void onDie();

private:
    double mass_;

    double angle_;
    double angularVelocity_;
    double torque_;

    Vec position_;
    Vec speed_;
    Vec velocity_;
    Vec force_;

    bool stepIsIgnored_;
    bool entityCollisionDetectionIsIgnored_;
    bool detectCollisionWith_(Body &otherBody);
    bool detectMapCollision_();
    void setWorld_(PhysicsWorld *gameWorld);
    void step_(double timeElapsedSec);

    bool isDead_;
};

#endif //__Body_H_
