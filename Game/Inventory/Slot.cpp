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

#include "Slot.h"
#include "App.h"
#include "Rect.h"
#include "Sprite.h"
#include "Property.h"

Slot::Slot(Texture* texture, Point location) : IDrawable()
{
    sprite_ = new Sprite(
            texture,
            Rect(0, 0, 40, 40)
    );

    sprite_->location->set(location);

    item_ = nullptr;
}

Slot::~Slot() {

}

void Slot::setItem(Item *item) {
    item_ = item;
}

Item *Slot::getItem() {
    return item_;
}

Item *Slot::removeItem() {
    Item* item = item_;
    item_ = nullptr;
    return item;
}

void Slot::draw(Canvas *canvas) {
    sprite_->draw(canvas);

    if (item_) {
        Point slotLocation = sprite_->location->get();

        glMatrixMode(GL_MODELVIEW);

        glTranslated(
                slotLocation.x,
                slotLocation.y,
                0
        );

        item_->sprite_->draw(canvas);

        glTranslated(
                -slotLocation.x,
                -slotLocation.y,
                0
        );

    }
}
