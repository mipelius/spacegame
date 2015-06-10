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

Inventory::Inventory() {
    // --- CANVAS ---

    canvas.setMargin(10, 10, 10, 10);
    canvas.w->set(500);
    canvas.h->set(300);
    canvas.anchor->set(Canvas::Anchor::TOP_LEFT);

    for (int x = 0; x < 5; x++) {
        for (int y = 0; y < 5; y++) {
            Sprite* sprite = new Sprite(App::getResources()->textures->inventorySlot, Rect(50+0+x*40,0+y*40,50+40+x*40,40+y*40));
            SpriteContainer* spriteContainer = new SpriteContainer();
            spriteContainer->addSprite(sprite);
            canvas.addDrawable(spriteContainer);
        }
    }

    for (int y = 0; y < 3; y++) {
        Sprite* sprite = new Sprite(App::getResources()->textures->inventoryUsableSlot, Rect(0, 80+y*40, 40, 80+40+y*40));
        SpriteContainer* spriteContainer = new SpriteContainer();
        spriteContainer->addSprite(sprite);
        canvas.addDrawable(spriteContainer);
    }

    Sprite* sprite = new Sprite(App::getResources()->textures->inventoryWeaponSlots, Rect(0, 0, 40, 80));
    SpriteContainer* spriteContainer = new SpriteContainer();
    spriteContainer->addSprite(sprite);
    canvas.addDrawable(spriteContainer);

    // --- CAMERA ---

    camera_ = new Camera();
    camera_->boundsRect->set(Rect(0, 0, 500, 300));
    camera_->areaRect->set(Rect(0, 0, 500, 300));

    canvas.setCamera(camera_);

}
