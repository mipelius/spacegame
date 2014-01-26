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

#ifndef __HumanController_H_
#define __HumanController_H_

#include "Controller.h"

class HumanController : public Controller {
private:
    bool _left;
    bool _right;
    bool _up;
    bool _down;
    bool _space;
    bool _enter;

    void _setAllFalse();
protected:
    void control(double timeElapsedSec);
    virtual void handleLeft();
    virtual void handleRight();
    virtual void handleUp();
    virtual void handleDown();
    virtual void handleSpace();
    virtual void handleEnter();
public:
    void left();
    void right();
    void up();
    void down();
    void space();
    void enter();
};


#endif //__HumanController_H_
