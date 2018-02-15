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

#ifndef __Camera_H_
#define __Camera_H_

#include "Rect.h"
#include "Vec.h"

class Camera {

public:
    Camera();

    void zoom(float amount);

    // getters and setters

    const Rect &getAreaRect() const;
    void setAreaRect(const Rect &areaRect);
    Vecf getPosition();
    void setPosition(const Vecf &value);

private:
    Rect areaRect_;

};

#endif //__Camera_H_
