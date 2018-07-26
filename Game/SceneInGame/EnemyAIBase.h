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



#ifndef SPACEGAME_ENEMYAIBASE_H
#define SPACEGAME_ENEMYAIBASE_H

#include "WeaponBase.h"
#include "Tile2DBehaviour.h"
#include "Body.h"
#include "CountDownTimer.h"
#include "EnemySpawner.h"

class EnemyAIBase : public Tile2DBehaviour, public ISerializable {

public:
    ~EnemyAIBase() override;

    void deserialize(const json::Object &jsonObject) override;

    void setTarget(Transform *target);
    Transform *getTarget() const;

    float getMaxDistance() const;
    void setMaxDistance(float maxDistance);

    template <class T>
    T* setWeapon();

protected:
    Body* body_;
    Transform* target_ = nullptr;

    void shootTarget_();

    float maxDistance_;

    void awake() override;
    void update() override;

    bool canSeeTarget_();

    WeaponBase* weapon_ = nullptr;

    int randomShootingDelay = 0;
    static const int MAX_RANDOM_SHOOTING_DELAY = 100;

    Timer shootingRandomizerTimer;
};

template<class T>
T* EnemyAIBase::setWeapon() {
    delete weapon_;
    auto weapon = new T();
    weapon_ = weapon;
    return weapon;
}

#endif //SPACEGAME_ENEMYAIBASE_H
