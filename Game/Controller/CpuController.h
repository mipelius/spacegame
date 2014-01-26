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

#ifndef __CpuController_H_
#define __CpuController_H_

class Brains;

#include "Controller.h"

class SpaceGameObject;

class CpuController : public Controller {
    friend class BrainCell;
private:
    Brains* _brains;
protected:
    void control(double timeElapsedSec);
public:
    CpuController(Brains* brains);
};

#endif //__CpuController_H_
