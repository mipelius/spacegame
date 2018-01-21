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

#ifndef __PhysicsWorld_H_
#define __PhysicsWorld_H_

class TileMap;
class Body;

#include <list>
#include "Vec.h"

class PhysicsWorld {
    friend class Tile2D;
    friend class Body;
    friend class TileMap;

public:
    const float defaultAirDensity = 0.001;
    const float defaultMetersPerPixel = 0.1;
    const Vecf defaultGForce = Vecf(0.0, 9.81);

    TileMap* getMap();

    void debugDraw();

    // getters and setters
    const Vecf &getGForce() const;
    void setGForce(const Vecf &gForce);
    float getMetersPerPixel() const;
    void setMetersPerPixel(float metersPerPixel);
    float getAirDensity() const;
    void setAirDensity(float airDensity);

private:
    PhysicsWorld();
    ~PhysicsWorld() = default;

    void step(float timeSeconds);
    void add(Body *body);
    void remove(Body *body);

private:
    Vecf gForce_;
    float metersPerPixel_;
    float airDensity_;

    std::list<Body *> bodies_;
    std::list<Body *> bodiesToRemove_;

    TileMap * map_;
    void detectCollision_(Body* body, float deltaTime);
};

#endif //__PhysicsWorld_H_
