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

#include "Tile2DComponent.h"
#include "Vec.h"
#include "Property.h"
#include "Event.h"
#include "MapCollisionEventArgs.h"
#include "PolygonCollider.h"

class PhysicsWorld;
class ColliderShape;
class BodyCollisionEventArgs;
class EventArgs;

class Body : public Tile2DComponent {
    friend class PhysicsWorld;
    friend class PolygonCollider;

public:
    // properties

    Property<float> const mass;

    Property<float> const angle;
    Property<float> const angularVelocity;

    Property<Vec> const position;
    Property<Vec> const velocity;
    Property<Vec> const force;

    // events

    Event<Body, BodyCollisionEventArgs> const bodyCollision;
    Event<Body, MapCollisionEventArgs> const mapCollision;

    // methods

    Body();

    void applyForce(Vec force);

    PhysicsWorld* getWorld();
    PolygonCollider* getCollider();

protected:
    PhysicsWorld *physicsWorld_;
    PolygonCollider* collider_;

private:
    void init() override;
    void onDestroy() override;

    float mass_;

    float angle_;
    float angularVelocity_;

    Vec position_;
    Vec velocity_;
    Vec force_;

    bool detectCollisionWith_(Body &otherBody);
    bool detectMapCollision_(float deltaTime);
    void setWorld_(PhysicsWorld *gameWorld);
    void step_(float timeElapsedSec);
};

#endif //__Body_H_
