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

Slot::Slot(Point location) : rect_(Rect(location.x, location.y, location.x + 40, location.y + 40))
{
    Sprite* sprite = new Sprite(
            App::getResources()->textures->inventorySlot,
            rect_
    );
    spriteContainer_ = new SpriteContainer();
    spriteContainer_->addSprite(sprite);
}

Slot::~Slot() {

}

void Slot::setItem(Item *item) {
    item_ = item;
    item_->spriteContainer_->location->set(Point(rect_.x1, rect_.y1));
}

Item *Slot::getItem() {
    return item_;
}

Item *Slot::removeItem() {
    Item* item = item_;
    item_ = nullptr;
    return item;
}
