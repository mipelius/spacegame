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

#include "Texture.h"
#include "DrawableBase.h"
#include "Rect.h"

class ColliderShape;
class ITexture;

class Sprite : public DrawableBase {

public:
    Sprite();

    // getters and setters

    const Rect &getRect() const;
    void setRect(const Rect &rect);
    ITexture *getTexturePtr() const;
    void setTexturePtr(ITexture *texturePtr);

protected:
    void drawActual(const Canvas &canvas) override;

private:
    Rect rect_;
    ITexture* texturePtr_;
};


#endif //__Sprite_H_
