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


#ifndef __PhysicsWorld_H_
#define __PhysicsWorld_H_

class TileMap;
class Body;
class PolygonCollider;

#include <list>
#include "Vec.h"
#include "ColliderLayerMatrix.h"

class PhysicsWorld {
    friend class Tile2D;
    friend class Body;
    friend class PolygonCollider;

public:
    const float defaultAirDensity       = 0.001f;
    const Vecf  defaultGForce           = Vecf(0.0f, 9.81f);

    void debugDraw();

    // getters and setters
    const Vecf &getGForce() const;
    void setGForce(const Vecf &gForce);
    float getAirDensity() const;
    void setAirDensity(float airDensity);

private:
    void init(std::vector<ColliderLayerMatrix::Rule> colliderMatrixRules);

    PhysicsWorld();
    ~PhysicsWorld();

    void step(float timeSeconds);

    ColliderLayerMatrix* colliderLayerMatrix_ = nullptr;

    Vecf gForce_;
    float airDensity_;

    std::list<Body *> bodies_;
    std::list<PolygonCollider *> colliders_;
};

#endif //__PhysicsWorld_H_
