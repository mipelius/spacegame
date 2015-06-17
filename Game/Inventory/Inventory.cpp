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

#include <SDL2/SDL_mouse.h>
#include "Inventory.h"
#include "Texture.h"
#include "Background.h"
#include "Camera.h"
#include "Sprite.h"
#include "DrawableGroup.h"
#include "Text.h"
#include "App.h"
#include "Slot.h"

int marginLeft;
int marginRight;
int marginTop;
int marginBottom;

Inventory::Inventory()
{
    for (int x = 0; x < 5; x++) {
        Slot* slot = new Slot(
                App::getResources()->textures->inventoryEquipableSlot,
                Point(x * 40, 0)
        );
        equipableSlots_.push_back(slot);
    }

    for (int x = 0; x < 5; x++) {
        for (int y = 1; y < 5; y++) {
            Slot* slot = new Slot(
                    App::getResources()->textures->inventorySlot,
                    Point(x * 40, y * 40)
            );
            inventorySlots_.push_back(slot);
        }
    }

    // --- TEST ITEMS ---
    Item* item1 = new Item(App::getResources()->textures->laserCannon);
    Item* item2 = new Item(App::getResources()->textures->plasmaCannon);

    inventorySlots_[0]->setItem(item1);
    inventorySlots_[7]->setItem(item2);

    // --- MOUSE ---

    mouseSelectedItem_ = nullptr;
    mouseSelectedSlot_ = nullptr;

    mouseState_ = 0;
    previousMouseState_ = 0;
}


void Inventory::drawActual(Canvas *canvas) {
    if (isBigInventoryVisible_) {
        for (std::vector<Slot*>::iterator i = inventorySlots_.begin(); i != inventorySlots_.end(); i++) {
            (*i)->draw(canvas);
        }
    }


    for (std::vector<Slot*>::iterator i = equipableSlots_.begin(); i != equipableSlots_.end(); i++) {
        (*i)->draw(canvas);
    }

    if (mouseSelectedItem_) {
        double x = mouseX_ - this->location->get().x - 20;
        double y = mouseY_ - this->location->get().y - 20;

        glTranslated(x, y, 0.0);
        mouseSelectedItem_->sprite_->draw(canvas);
        glTranslated(-x, -y, 0.0);
    }
}

void Inventory::checkMouseActions() {
    mouseState_ = SDL_GetMouseState(&mouseX_, &mouseY_);

    Point mouseLocation = Point(mouseX_, mouseY_);

    // --- LEFT MOUSE BUTTON ---

    if (mouseState_ == 1 && previousMouseState_ == 0) {
        Slot* slot = getSlot(mouseLocation);

        if (mouseSelectedItem_ == nullptr) {
            if (slot) {
                mouseSelectedItem_ = slot->removeItem();
                mouseSelectedSlot_ = slot;
            }
        }
        else {
            if (slot) {
                Item* previousItem = slot->getItem();

                if (previousItem) {
                    mouseSelectedSlot_->setItem(previousItem);
                }

                slot->setItem(mouseSelectedItem_);

                mouseSelectedItem_ = nullptr;
                mouseSelectedSlot_ = nullptr;
            }
            else {
                // remove item ?
            }
        }
    }

    // --- RIGHT MOUSE BUTTON ---

    if (mouseState_ == 4) {
        // ---
    }

    previousMouseState_ = mouseState_;
}


Slot *Inventory::getSlot(Point location) {
    if (
            location.x < this->location->get().x            ||
            location.x > this->location->get().x + 40 * 5   ||
            location.y < this->location->get().y            ||
            location.y > this->location->get().y + 40 * 5
    ) {
        return nullptr;
    }

    Point actualLocation = location -
                    Vector(
                            this->location->get().x,
                            this->location->get().y
                    );

    for (std::vector<Slot*>::iterator i = equipableSlots_.begin(); i != equipableSlots_.end(); i++) {
        Slot* slot = (*i);

        if (slot->sprite_->getActualRect().hasPointInside(actualLocation)) {
            return slot;
        }
    }

    if (isBigInventoryVisible_) {
        for (std::vector<Slot*>::iterator i = inventorySlots_.begin(); i != inventorySlots_.end(); i++) {
            Slot* slot = (*i);

            if (slot->sprite_->getActualRect().hasPointInside(actualLocation)) {
                return slot;
            }
        }
    }

    return nullptr;
}

void Inventory::toggleBigInventoryVisibility() {
    isBigInventoryVisible_ = !isBigInventoryVisible_;
    cancelMouseSelection();
}

void Inventory::cancelMouseSelection() {
    mouseState_ = 0;
    previousMouseState_ = 0;

    if (mouseSelectedItem_) {
        mouseSelectedSlot_->setItem(mouseSelectedItem_);
        mouseSelectedItem_ = nullptr;
        mouseSelectedSlot_ = nullptr;
    }
}
