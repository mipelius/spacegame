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

#ifndef __Tile2DBehaviour_H
#define __Tile2DBehaviour_H

#include "Tile2DComponent.h"

class Tile2DBehaviour : public Tile2DComponent {
    friend class Tile2D;

protected:
    void init() final;
    void onDestroy() final;

    virtual void awake() = 0;
    virtual void update() = 0;
};


#endif //__Tile2DBehaviour_H
