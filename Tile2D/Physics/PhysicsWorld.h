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
#include "Property.h"

class PhysicsWorld {
    friend class Tile2D;
    friend class Body;
    friend class TileMap;

public:
    Property<Vec> const gForce;
    Property<double> const metersPerPixel;
    Property<double> const airDensity;

    const double defaultAirDensity = 0.001;
    const double defaultMetersPerPixel = 0.1;
    const Vec defaultGForce = Vec(0.0, 9.81);

    void debugDraw();

private:
    PhysicsWorld();
    ~PhysicsWorld() = default;

    void step(double timeSeconds);
    void add(Body *body);
    void remove(Body *body);

    Vec gForce_;
    double metersPerPixel_;
    double airDensity_;

    std::list<Body *> bodies_;
    TileMap * map_;
    void detectCollision_(Body* body);
};

#endif //__PhysicsWorld_H_
