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

#ifndef __PartialLightMap_H_
#define __PartialLightMap_H_

class TileMap;

#include "Array2d.h"

class PartialLightMap : public Array2d<unsigned char> {

public:
    PartialLightMap(int w, int h);

    void setCenterLocation(int x, int y);

    void update(TileMap* map);

    int getX() const;
    int getY() const;

    bool needsUpdate();

private:

    bool needsUpdate_;

    int x_;
    int y_;

    void updateInternal(unsigned char lastLight, int currentX, int currentY, TileMap* map);

};
#endif //__PartialLightMap_H_
