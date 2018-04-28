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
#include "Tile2DComponent.h"
#include "IWeapon.h"

struct WeaponSlot {
    IWeapon* weapon;
    Texture* inventoryTexturePtr;
    Texture* inWorldTexturePtr;
    int tag;
};

class WeaponSystem : public Tile2DComponent {
public:
    void shoot();

    template <class T>
    T* attachWeapon(Texture* inventoryTexturePtr, Texture* inWorldTexturePtr, int tag);

    const std::vector<WeaponSlot>& getWeaponSlots() const;

    int getCurrentWeaponSlot() const;
    void setCurrentWeaponSlot(int currentWeapon);

protected:
    void init() override;
    void onDestroy() override;

private:
    std::vector<WeaponSlot> weaponSlots_;
    int currentWeapon_ = 0;
    Power* power_;
    Body* body_;
};

// --- Template method implemention ---

template <class T>
T* WeaponSystem::attachWeapon(Texture* inventoryTexturePtr, Texture* inWorldTexturePtr, int tag) {
    auto weapon = new T();

    WeaponSlot weaponSlot = {weapon, inventoryTexturePtr, inWorldTexturePtr, tag};
    weaponSlots_.push_back(weaponSlot);

    return weapon;
}

#endif //SPACEGAME_WEAPONSYSTEM_H
