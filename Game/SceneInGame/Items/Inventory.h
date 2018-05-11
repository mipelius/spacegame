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

#ifndef SPACEGAME_WEAPONSYSTEM_H
#define SPACEGAME_WEAPONSYSTEM_H

#include <vector>
#include "Texture.h"
#include "Tile2DBehaviour.h"
#include "ItemBase.h"
#include "Body.h"

struct ItemInfo {
    ItemBase* item;
    Texture* inventoryTexturePtr;
    int tag;
    bool automatic;
};

class Inventory : public Tile2DBehaviour {
public:
    template <class T>
    T *attachItem(Texture *inventoryTexturePtr, int tag, bool automatic);

    const std::vector<ItemInfo>& getItemInfos() const;

    int getSelectedItem() const;
    void selectItem(int itemNumber);

    ItemBase* getItem(int tag);

protected:
    void awake() override;
    void update() override;
    void lateUpdate() override;

    void onDestroy() override;

private:
    std::vector<ItemInfo> itemInfos_;
    int selectedItem_ = 0;

    void useSelectedItem_();
};

// --- Template method implementation ---

template <class T>
T* Inventory::attachItem(Texture *inventoryTexturePtr, int tag, bool automatic) {
    auto item = new T();

    ItemInfo itemInfo = {item, inventoryTexturePtr, tag, automatic};
    itemInfos_.push_back(itemInfo);

    return item;
}

#endif //SPACEGAME_WEAPONSYSTEM_H
