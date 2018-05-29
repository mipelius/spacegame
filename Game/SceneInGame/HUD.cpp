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
#include "Resources.h"
#include "Power.h"
#include "GameObject.h"

void HUD::awake() {
    GameObject *healthBar = Tile2D::createGameObject();

    healthSprite_ = healthBar->attachComponent<Sprite>();
    healthSprite_->setRect({0.0f, 0.0f, 10.0f, 10.0f});
    healthSprite_->setColor({0.7f, 0.0f, 0.0f});
    healthSprite_->setIsUIDrawable(true);
    healthSprite_->setOpacity(0.75f);

    GameObject *powerBar = Tile2D::createGameObject();

    powerSprite_ = powerBar->attachComponent<Sprite>();
    powerSprite_->setRect({0.0f, 0.0f, 200.0f, 10.0f});
    powerSprite_->setColor({0.3f, 0.7f, 0.0f});
    powerSprite_->setIsUIDrawable(true);
    powerSprite_->setOpacity(0.75f);

    updateItemSlots_();
}

void HUD::update() {
    if (!player_) {
        return;
    }
    {   // HEALTH
        Rect rect = healthSprite_->getRect();
        rect.x2 = (float)player_->getComponent<Health>()->getHealth();
        healthSprite_->setRect(rect);
    }
    {   // POWER
        Rect rect = powerSprite_->getRect();
        rect.x2 = (float)player_->getComponent<Power>()->getPower();
        powerSprite_->setRect(rect);
    }
    {   // ITEM SLOTS
        updateItemSlots_();
    }

}

void HUD::lateUpdate() {

}

void HUD::setPlayer(GameObject *player) {
    player_ = player;
    inventory_ = player->getComponent<Inventory>();
}

void HUD::onDestroy() {
    Tile2DBehaviour::onDestroy();
    healthSprite_->gameObject()->destroy();
    powerSprite_->gameObject()->destroy();
    for (auto itemSlot : itemSlots_) {
        itemSlot.itemSlotSprite->gameObject()->destroy();
    }
}

void HUD::updateItemSlots_() {
    if (inventory_ == nullptr) {
        return;
    }

    auto itemCount = (int) inventory_->getItemInfos().size();

    if (itemCount != itemSlots_.size()) {
        for (auto itemSlot : itemSlots_) {
            itemSlot.itemCountText->gameObject()->destroy();
        }
        itemSlots_.clear();

        float width = 40.0f;
        float height = 40.0f;
        float margin = 2.0f;
        Vecf offset = {20.0f, 20.0f};

        for (auto i = 0u; i < itemCount; ++i) {
            GameObject *itemSlot = Tile2D::createGameObject();
            itemSlot->transform().setPosition(Vecf((width + margin) * i, 0) + offset);

            auto itemSlotSprite = itemSlot->attachComponent<Sprite>();
            itemSlotSprite->setRect({0.0f, 0.0f, width, height});
            itemSlotSprite->setIsUIDrawable(true);
            itemSlotSprite->setTexturePtr(Tile2D::resources().textures["inventory_slot"]);
            itemSlotSprite->setSortingLayer(SortingLayers::HUD_WeaponSlot);

            auto itemSprite = itemSlot->attachComponent<Sprite>();
            itemSprite->setRect({5.0f, 5.0f, width - 5.0f, height - 5.0f});
            itemSprite->setIsUIDrawable(true);
            itemSprite->setSortingLayer(SortingLayers::HUD_Weapon);

            auto itemSlotText = itemSlot->attachComponent<Text>();
            itemSlotText->setFontPtr(Tile2D::resources().fonts["smallfont"]);
            itemSlotText->setIsUIDrawable(true);
            itemSlotText->setString(std::to_string(i + 1));
            itemSlotText->setSortingLayer(SortingLayers::HUD_Text);
            itemSlotText->setFontSize(1.0f);
            itemSlotText->setHorizontalAlignment(Text::HorizontalAlignment::right);
            itemSlotText->setVerticalAlignment(Text::VerticalAlignment::bottom);
            itemSlotText->localTransform().setPosition({width - 4.0f, height - 4.0f});

            auto itemCountText = itemSlot->attachComponent<Text>();
            itemCountText->setFontPtr(Tile2D::resources().fonts["smallfont"]);
            itemCountText->setIsUIDrawable(true);
            itemCountText->setSortingLayer(SortingLayers::HUD_Text);
            itemCountText->setFontSize(1.0f);
            itemCountText->setHorizontalAlignment(Text::HorizontalAlignment::right);
            itemCountText->setVerticalAlignment(Text::VerticalAlignment::top);
            itemCountText->localTransform().setPosition({width - 4.0f, 4.0f});

            itemSlots_.push_back({itemSlotSprite, itemSprite, itemSlotText, itemCountText});
        }

        offset += Vecf((width + margin) * itemCount + 10.0f, 0.0);
        healthSprite_->transform()->setPosition(offset + Vecf(0.0f, 5.0f));
        powerSprite_->transform()->setPosition(offset + Vecf(0.0f, height - 15.0f));
    }

    auto itemSlotInfos = inventory_->getItemInfos();
    auto selectedItemSlot = inventory_->getSelectedItem();

    for (auto i = 0u; i < itemSlotInfos.size(); ++i) {
        auto itemInfo = itemSlotInfos[i];
        auto isActivated = itemInfo.item->isActivated();
        auto texture = i == selectedItemSlot ?
                       Tile2D::resources().textures["inventory_selected_slot"] :
                       Tile2D::resources().textures["inventory_slot"];

        itemSlots_[i].itemSlotSprite->setTexturePtr(texture);
        itemSlots_[i].itemSprite->setTexturePtr(inventory_->getItemInfos()[i].inventoryTexturePtr);

        itemSlots_[i].itemSlotText->setIsVisible(isActivated);
        itemSlots_[i].itemSprite->setIsVisible(isActivated);
        itemSlots_[i].itemCountText->setIsVisible(isActivated);

        if (isActivated) {
            const auto& count = itemInfo.item->getCount();
            auto itemCountString = count >= 0 ? std::to_string(count) : "";
            itemSlots_[i].itemCountText->setString(itemCountString);
        }
    }
}
