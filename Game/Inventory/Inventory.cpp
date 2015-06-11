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

#include "Inventory.h"
#include "Texture.h"
#include "Background.h"
#include "Camera.h"
#include "Sprite.h"
#include "SpriteContainer.h"
#include "Text.h"
#include "App.h"
#include "Slot.h"

int marginLeft;
int marginRight;
int marginTop;
int marginBottom;

Inventory::Inventory() {
    // --- CANVAS ---

    canvas.setMargin(10, 10, 10, 10);
    canvas.w->set(500);
    canvas.h->set(300);
    canvas.anchor->set(Canvas::Anchor::TOP_LEFT);

    for (int x = 0; x < 5; x++) {
        for (int y = 0; y < 5; y++) {
            Slot* slot = new Slot(
                    Point(x * 43, y * 43)
            );
            slots_.push_back(slot);
            canvas.addDrawable(slot->spriteContainer_);
        }
    }


    // --- TEST ITEMS ---
    Item* item1 = new Item(App::getResources()->textures->laserCannon);

    Item* item2 = new Item(App::getResources()->textures->plasmaCannon);

    canvas.addDrawable(item1->spriteContainer_);
    canvas.addDrawable(item2->spriteContainer_);
    slots_[0]->setItem(item1);
    slots_[7]->setItem(item2);

    // ---

    selectedItem_ = nullptr;


    // --- CAMERA ---

    camera_ = new Camera();
    camera_->boundsRect->set(Rect(0, 0, 500, 300));
    camera_->areaRect->set(Rect(0, 0, 500, 300));

    canvas.setCamera(camera_);

}

void Inventory::mouseButtonDown(int x, int y) {
    if (selectedItem_) {
        selectedItem_->spriteContainer_->location->set(Point(x - 30, y - 30));
    }
    else {
        for (std::vector<Slot*>::iterator i = slots_.begin(); i != slots_.end(); i++) {
            Slot* slot = (*i);
            if (
                    slot->rect_.hasPointInside(Point(x - 10, y - 10))
            ) {
                selectedItem_ = slot->getItem();
                selectedSlot_ = slot;
                break;
            }
        }
    }
}

void Inventory::mouseButtonReleased(int x, int y) {
    if (selectedItem_) {
        for (std::vector<Slot*>::iterator i = slots_.begin(); i != slots_.end(); i++) {
            Slot* slot = (*i);
            if (
                    slot->rect_.hasPointInside(Point(x - 10, y - 10))
                    ) {
                if (selectedSlot_ != slot) {
                    selectedSlot_->removeItem();
                }

                Item* item = slot->removeItem();

                if (item) {
                    selectedSlot_->setItem(item);
                }

                slot->setItem(selectedItem_);
                selectedItem_ = nullptr;
                return;
            }
        }
        selectedSlot_->setItem(selectedItem_);
        selectedItem_ = nullptr;
    }

}
