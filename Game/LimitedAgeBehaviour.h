// This file is part of SpaceGame.
// Copyright (C) 2018 Miika Pelkonen
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

#ifndef __AgeDependentBehaviour_H
#define __AgeDependentBehaviour_H

#include <SDL2/SDL_bits.h>
#include <cfloat>
#include "Tile2DBehaviour.h"

class LimitedAgeBehaviour : public Tile2DBehaviour {

public:
    virtual float age();
    float TTL = FLT_MAX;

protected:
    void awake() override;
    void update() override;

private:
    Uint32 awakeTimeStamp = 0;
};


#endif //__AgeDependentBehaviour_H
