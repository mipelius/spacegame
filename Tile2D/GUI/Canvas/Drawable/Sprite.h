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

#ifndef __Sprite_H_
#define __Sprite_H_

class CollisionShape;
class ITexture;

#include "Rect.h"

class Sprite {
private:
    ITexture* texture_;
    Rect rect_;
public:
    Sprite(ITexture* texture, Rect rect);
    void render(Point locationPoint, double angle);
};


#endif //__Sprite_H_
