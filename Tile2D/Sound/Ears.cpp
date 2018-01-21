// This file is part of SpaceGame.
// Copyright (C) 2014 Miika Pelkonen
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

#include "Vec.h"
#include "Ears.h"

Ears::Ears() :
    position_       (   Vecf(0, 0) ),
    maxDistance_    (   0.0          )
{

}

Ears::~Ears() {

}

// getters and setters

const Vecf &Ears::getPosition() const {
    return position_;
}

void Ears::setPosition(const Vecf &position) {
    position_ = position;
}

float Ears::getMaxDistance() const {
    return maxDistance_;
}

void Ears::setMaxDistance(float maxDistance) {
    maxDistance_ = maxDistance;
}
