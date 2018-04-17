// MIT License
//
// This file is part of SpaceGame.
// Copyright (c) 2014-2018 Miika Pelkonen
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


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
