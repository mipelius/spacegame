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

#include "WeaponBase.h"

bool WeaponBase::shoot(Power *power, const Vecf& from, const Vecf& direction, const Vecf& shooterVelocity) {
    if (!isActivated_) {
        return false;
    }
    if (power == nullptr) {
        return tryShoot(from, direction, shooterVelocity);
    }
    if (power->getPower() < powerConsumption_) {
        return false;
    }
    if (tryShoot(from, direction, shooterVelocity)) {
        power->consume(powerConsumption_);
        return true;
    }
    return false;
}

int WeaponBase::getPowerConsumption() const {
    return powerConsumption_;
}

void WeaponBase::setPowerConsumption(int powerConsumption) {
    powerConsumption_ = powerConsumption;
}

bool WeaponBase::isActivated() const {
    return isActivated_;
}

void WeaponBase::setIsActivated(bool isActivated) {
    isActivated_ = isActivated;
}

