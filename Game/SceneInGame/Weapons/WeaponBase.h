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

#ifndef SPACEGAME_WEAPONBASE_H
#define SPACEGAME_WEAPONBASE_H

#include "IWeapon.h"

class WeaponBase : public IWeapon {
public:
    bool shoot(Power *power, const Vecf& from, const Vecf& direction, const Vecf& shooterVelocity) final;

    int getPowerConsumption() const;
    void setPowerConsumption(int powerConsumption);

    bool isActivated() const;
    void setIsActivated(bool isActivated);

protected:
    virtual bool tryShoot(const Vecf& from, const Vecf& direction, const Vecf& shooterVelocity) = 0;

private:
    bool isActivated_ = false;
    int powerConsumption_ = 0;

};


#endif //SPACEGAME_WEAPONBASE_H
