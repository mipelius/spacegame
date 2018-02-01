// This file is part of SpaceGame.
// Copyright (C) 2017 Miika Pelkonen
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

#ifndef __PlayerController_H
#define __PlayerController_H

#include "Body.h"
#include "Sprite.h"
#include "Tile2DBehaviour.h"
#include "Timer.h"

class PlayerController : public Tile2DBehaviour {

public:
    float moveForce;

protected:
    void awake() override;
    void update() override;
    void lateUpdate() override;

private:
    Timer shootTimer;
    Timer bombTimer;
    Timer lightTimer;

    Body* body_;
    Sprite* sprite_;

    void shoot_();
    void shootOnce_(Vecf offset);
    void dropBomp_();
    void dropLight_();

};

#endif //__PlayerController_H
