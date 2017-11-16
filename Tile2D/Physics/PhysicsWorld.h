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

class WorldMap;
class Body;

#include "Vector.h"
#include "Property.h"

class PhysicsWorld {
    friend class Tile2D;
public:
    void step(double timeSeconds);
    void add(Body &body);

    void setMap(WorldMap &map);
    WorldMap & getMap();

    Property<Vector> const gForce;
    Property<double> const metersPerPixel;
    Property<double> const airDensity;

    const double defaultAirDensity = 0.001;
    const double defaultMetersPerPixel = 0.001;
    const Vector defaultGForce = Vector(0.0, -9.81);

private:
    PhysicsWorld();
    PhysicsWorld(Vector gForce, double metersPerPixel, double airDensity);
    ~PhysicsWorld() = default;

    Vector gForce_;
    double metersPerPixel_;
    double airDensity_;

    std::list<Body *> bodies_;
    WorldMap * map_;
    void detectCollision_(Body &body);
};

#endif //__PhysicsWorld_H_
