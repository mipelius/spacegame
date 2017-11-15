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

#ifndef __Tile2DObject_H
#define __Tile2DObject_H

class Tile2DObject {
    friend class Tile2D;
private:
    bool createdByTile2D_ = false;

public:
    Tile2DObject()                                      = default;
    ~Tile2DObject()                                     = default;
    Tile2DObject(const Tile2DObject& other)             = delete;
    Tile2DObject& operator=(const Tile2DObject& other)  = delete;
    Tile2DObject(const Tile2DObject&& other)            = delete;
    Tile2DObject& operator=(const Tile2DObject&& other) = delete;

    void destroy();
};


#endif //__Tile2DObject_H
