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

#include "Power.h"
#include "Body.h"
#include "Tile2DMath.h"
#include "WeaponSystem.h"

void WeaponSystem::shoot() {
    if (weaponInfos_.empty()) {
        return;
    }
    auto weaponInfo = weaponInfos_[currentWeapon_];

    auto weapon = weaponInfo.weapon;

    weapon->shoot(
            power_,
            transform()->getPosition(),
            Vecf::byAngle(transform()->getRotation(), 1.0f),
            body_->getVelocity()
    );
}

const std::vector<WeaponInfo> &WeaponSystem::getWeaponInfos() const {
    return weaponInfos_;
}

void WeaponSystem::init() {
    power_ = gameObject()->getComponent<Power>();
    body_ = gameObject()->getComponent<Body>();
}

void WeaponSystem::onDestroy() {
    for (auto weaponInfo : weaponInfos_) {
        delete weaponInfo.weapon;
    }
}

int WeaponSystem::getCurrentWeaponSlot() const {
    return currentWeapon_;
}

void WeaponSystem::setCurrentWeaponSlot(int currentWeapon) {
    currentWeapon;
    Mathi::clamp(currentWeapon, 0, (int)(weaponInfos_.size()) -1);
    if (weaponInfos_[currentWeapon].weapon->isActivated()) {
        currentWeapon_ = currentWeapon;
    }
}