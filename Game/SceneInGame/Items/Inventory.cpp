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

#include "AudioManager.h"
#include "Tile2D.h"
#include "Power.h"
#include "Input.h"
#include "Tile2DMath.h"
#include "Inventory.h"
#include "Resources.h"

void Inventory::useSelectedItem_() {
    if (itemInfos_.empty()) {
        return;
    }
    auto itemInfo = itemInfos_[selectedItem_];
    auto item = itemInfo.item;

    item->use(gameObject());
}

const std::vector<ItemInfo> &Inventory::getItemInfos() const {
    return itemInfos_;
}

void Inventory::onDestroy() {
    Tile2DBehaviour::onDestroy();

    for (auto weaponInfo : itemInfos_) {
        delete weaponInfo.item;
    }
}

int Inventory::getSelectedItem() const {
    return selectedItem_;
}

void Inventory::selectItem(int itemNumber) {
    itemNumber;
    Mathi::clamp(itemNumber, 0, (int)(itemInfos_.size()) -1);
    if (
            itemInfos_[itemNumber].item->isActivated() &&
            selectedItem_ != itemNumber
    ) {
        selectedItem_ = itemNumber;
        AudioManager::getInstance()->play(itemSelectAudioClip_);
    }
}

ItemBase *Inventory::getItem(int tag) {
    for (auto itemInfo : itemInfos_) {
        if (itemInfo.tag == tag) {
            return itemInfo.item;
        }
    }
    return nullptr;
}



void Inventory::awake() {

}

void Inventory::update() {
    // weapon slot selection
    const auto& keyboard = Tile2D::input().keyboard();

    if (keyboard.keyPressed(SDL_SCANCODE_1)) {
        selectItem(0);
    }
    if (keyboard.keyPressed(SDL_SCANCODE_2)) {
        selectItem(1);
    }
    if (keyboard.keyPressed(SDL_SCANCODE_3)) {
        selectItem(2);
    }
    if (keyboard.keyPressed(SDL_SCANCODE_4)) {
        selectItem(3);
    }
    if (keyboard.keyPressed(SDL_SCANCODE_5)) {
        selectItem(4);
    }
    if (keyboard.keyPressed(SDL_SCANCODE_6)) {
        selectItem(5);
    }
}

void Inventory::lateUpdate() {
    if (itemInfos_[selectedItem_].automatic) {
        if (Tile2D::input().keyboard().keyState(SDL_SCANCODE_SPACE)) {
            useSelectedItem_();
        }
    }
    else {
        if (Tile2D::input().keyboard().keyPressed(SDL_SCANCODE_SPACE)) {
            useSelectedItem_();
        }
    }
}

void Inventory::setItemTexture(int tag, Texture *inventoryTexturePtr) {
    for (auto& itemInfo : itemInfos_) {
        if (itemInfo.tag == tag) {
            itemInfo.inventoryTexturePtr = inventoryTexturePtr;
        }
    }
}

void Inventory::deserialize(const json::Object &jsonObject) {
    if (jsonObject.HasKey("itemInfos")) {
        itemInfos_.clear();

        auto itemInfosJson = jsonObject["itemInfos"].ToArray();
        for (const json::Value& itemInfoJsonValue : itemInfosJson) {
            auto itemInfoJsonObject = itemInfoJsonValue.ToObject();

            auto itemJsonObject = itemInfoJsonObject["item"].ToObject();
            auto item = Tile2D::reflector().instantiate<ItemBase>(itemJsonObject);

            auto inventoryTextureName = itemInfoJsonObject["inventoryTexture"].ToString();
            auto inventoryTexture = Tile2D::resources().textures[inventoryTextureName];

            auto tag = itemInfoJsonObject["tag"].ToInt();

            auto automatic = itemInfoJsonObject["automatic"].ToBool();

            ItemInfo itemInfo = {
                    item,
                    inventoryTexture,
                    tag,
                    automatic
            };

            itemInfos_.push_back(itemInfo);
        }
    }
    if (jsonObject.HasKey("itemSelectAudioClip")) {
        auto itemSelectAudioClipName = jsonObject["itemSelectAudioClip"];
        itemSelectAudioClip_ = Tile2D::resources().audioClips[itemSelectAudioClipName];
    }
}

Tile2DComponent *Inventory::clone() {
    return new Inventory(*this);
}

Inventory::Inventory(Inventory &other) : Tile2DBehaviour(other) {
    *this = other;
    itemInfos_.clear();
    for (auto itemInfo : other.itemInfos_) {
        ItemInfo itemInfoCopy = {
                itemInfo.item->clone(),
                itemInfo.inventoryTexturePtr,
                itemInfo.tag,
                itemInfo.automatic
        };
        itemInfos_.push_back(itemInfoCopy);
    }
}
