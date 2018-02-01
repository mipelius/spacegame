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
#include "PolygonCollider.h"

class Body : public Tile2DComponent {
    friend class PhysicsWorld;
    friend class PolygonCollider;

public:
    Body();

    void applyForce(Vecf force);

    // getters and setters

    float getMass() const;
    void setMass(float mass);
    float getAngularVelocity() const;
    void setAngularVelocity(float angularVelocity);
    const Vecf &getVelocity() const;
    void setVelocity(const Vecf &velocity);
    float getDrag() const;
    void setDrag(float drag);
    float getGravityFactor() const;
    void setGravityFactor(float gravityFactor);

private:
    void init() override;
    void onDestroy() override;

    float mass_;
    float angularVelocity_;

    Vecf velocity_;
    Vecf force_;
    float drag_;
    float gravityFactor_;

    void step_(float timeElapsedSec);
};

#endif //__Body_H_
