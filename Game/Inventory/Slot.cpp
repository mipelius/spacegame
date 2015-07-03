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

Slot::Slot(Point location, int slotNumber) : IDrawable()
{
    if (slotNumber > 0) {
        sprite_ = new Sprite(
            App::getResources()->textures->inventoryEquipableSlot,
            Rect(0, 0, 40, 40)
        );

        selectedSprite_ = new Sprite(
                App::getResources()->textures->inventorySelectedSlot,
                Rect(0, 0, 40, 40)
        );

        slotNumberText = new Text(App::getResources()->other->smallFont);
        slotNumberText->location.set(location + Vector(3, 2));
//        slotNumberText->string.set(std::to_string(slotNumber));
//
//        selectedSprite_->location->set(location);
    }
    else {
        sprite_ = new Sprite(
                App::getResources()->textures->inventorySlot,
                Rect(0, 0, 40, 40)
        );

        selectedSprite_ = nullptr;

        slotNumberText = nullptr;

    }

//    sprite_->location->set(location);

    item_ = nullptr;
    isSelected_ = false;
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
    if (selectedSprite_) {
        if (isSelected_) {
            selectedSprite_->draw(canvas);
        }
        else {
            sprite_->draw(canvas);
        }
    }
    else {
        sprite_->draw(canvas);
    }

    if (slotNumberText) {
        slotNumberText->draw(canvas);
    }

    if (item_) {
        glTranslate_();
        item_->sprite_->draw(canvas);
        glUnTranslate_();
    }
}

void Slot::select() {
    isSelected_ = true;
}

void Slot::unselect() {
    isSelected_ = false;
}

void Slot::glTranslate_() {
    Point slotLocation = sprite_->location.get();

    glMatrixMode(GL_MODELVIEW);

    glTranslated(
            slotLocation.x,
            slotLocation.y,
            0
    );
}

void Slot::glUnTranslate_() {
    Point slotLocation = sprite_->location.get();

    glTranslated(
            -slotLocation.x,
            -slotLocation.y,
            0
    );
}
