// This file is part of SpaceGame.
// Copyright (C) 2015 Miika Pelkonen
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

#ifndef __Slot_H_
#define __Slot_H_

#include "Item.h"

#include "Point.h"
#include "Sprite.h"
#include "Texture.h"

class Slot : public IDrawable {

friend class Inventory;

    Slot(Texture* texture, Point location);
    ~Slot();
    void setItem(Item* item);
    Item* getItem();
    Item* removeItem();

private:
    Item* item_;
    Sprite* sprite_;

protected:
    virtual void draw(Canvas *canvas);

};


#endif //__Slot_H_
