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

#include "Tile2D.h"
#include "Power.h"
#include "Input.h"
#include "Tile2DMath.h"
#include "Inventory.h"

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
    if (itemInfos_[itemNumber].item->isActivated()) {
        selectedItem_ = itemNumber;
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
