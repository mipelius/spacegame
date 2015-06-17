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

#ifndef __Inventory_H_
#define __Inventory_H_

#include "Canvas.h"
#include "DrawableBase.h"
#include <vector>
#include <SDL2/SDL_types.h>
#include "SimpleBooleanProperty.h"

class Camera;
class Slot;
class Item;

class Inventory : public DrawableBase {

public:
    Inventory();
    void checkMouseActions();
    void toggleBigInventoryVisibility();

    void selectSlot(int slotNumber);
    Slot* getSelectedSlot();

protected:
    virtual void drawActual(Canvas *canvas) override;

private:
    Slot* selectedSlot_;

    Slot* getSlot(Point location);

    std::vector<Slot*> inventorySlots_;
    std::vector<Slot*> equipableSlots_;

    Camera* camera_;

    bool isBigInventoryVisible_;

    // --- MOUSE ---

    Item* mouseSelectedItem_;
    Slot* mouseSelectedSlot_;

    Uint32 mouseState_;
    Uint32 previousMouseState_;

    int mouseX_, mouseY_;

    void cancelMouseSelection();
};

#endif //__Inventory_H_
