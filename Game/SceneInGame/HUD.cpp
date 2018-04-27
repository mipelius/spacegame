// MIT License
//
// This file is part of SpaceGame.
// Copyright (c) 2014-2018 Miika Pelkonen
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


#include "Text.h"
#include "SortingLayers.h"
#include "t2Time.h"
#include "Tile2D.h"
#include "HUD.h"
#include "resources.h"

void HUD::awake() {
    // -- weapon slots --
    int weaponSlots = 10;
    float width = 40;
    float height = 40;
    float margin = 2;
    Vecf offset = {20.0f, 20.0f};

    for (auto i = 0u; i < weaponSlots; ++i) {
        GameObject *weaponSlot = Tile2D::createGameObject();
        weaponSlot->transform().setPosition(Vecf((width + margin) * i, 0) + offset);

        auto weaponSlotSprite = weaponSlot->attachComponent<Sprite>();
        weaponSlotSprite->setRect({0.0f, 0.0f, width, height});
        weaponSlotSprite->setIsUIDrawable(true);
        weaponSlotSprite->setTexturePtr(Tile2D::resources().textures["inventory_slot"]);
        weaponSlotSprite->setSortingLayer(SortingLayers::UI_Button);

        auto weaponSlotText = weaponSlot->attachComponent<Text>();
        weaponSlotText->setFontPtr(Tile2D::resources().fonts["smallfont"]);
        weaponSlotText->setIsUIDrawable(true);
        weaponSlotText->setString(std::to_string(i));
        weaponSlotSprite->setSortingLayer(0);
        weaponSlotText->setSortingLayer(SortingLayers::UI_ButtonText);
        weaponSlotText->setFontSize(1.0f);
        weaponSlotText->setHorizontalAlignment(Text::HorizontalAlignment::right);
        weaponSlotText->setVerticalAlignment(Text::VerticalAlignment::bottom);
        weaponSlotText->localTransform().setPosition({width - 4.0f, height - 4.0f});

        weaponSlotSprites_.push_back(weaponSlotSprite);
    }

    offset += Vecf((width + margin) * weaponSlots + 10.0f, 0.0);

    // -- health bar --
    GameObject *healthBar = Tile2D::createGameObject();
    healthBar->transform().setPosition(offset + Vecf(0.0f, 5.0f));

    healthSprite_ = healthBar->attachComponent<Sprite>();
    healthSprite_->setRect({0.0f, 0.0f, 10.0f, 10.0f});
    healthSprite_->setColor({0.7f, 0.0f, 0.0f});
    healthSprite_->setIsUIDrawable(true);

    // -- power bar --
    GameObject *powerBar = Tile2D::createGameObject();
    powerBar->transform().setPosition(offset + Vecf(0.0f, height - 15.0f));

    powerSprite_ = powerBar->attachComponent<Sprite>();
    powerSprite_->setRect({0.0f, 0.0f, 200.0f, 10.0f});
    powerSprite_->setColor({0.3f, 0.7f, 0.0f});
    powerSprite_->setIsUIDrawable(true);
}

void HUD::update() {
    if (!player_) {
        return;
    }
    Rect rect = healthSprite_->getRect();
    rect.x2 = (float)player_->getComponent<Health>()->getHealth();
    healthSprite_->setRect(rect);
}

void HUD::lateUpdate() {

}

void HUD::setPlayer(GameObject *player) {
    player_ = player;
}

void HUD::onDestroy() {
    Tile2DBehaviour::onDestroy();
    healthSprite_->gameObject()->destroy();
    powerSprite_->gameObject()->destroy();
    for (auto weaponSlotSprite : weaponSlotSprites_) {
        weaponSlotSprite->gameObject()->destroy();
    }
}
