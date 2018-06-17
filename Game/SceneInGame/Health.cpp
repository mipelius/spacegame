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
#include "t2Time.h"
#include "Health.h"
#include "Tile2DMath.h"
#include "GameObject.h"

void Health::awake() {
    reset();
}

void Health::update() {
    heal(Tile2D::time().getDeltaTime() * autoHealingRate_);
}

void Health::lateUpdate() { }

int Health::getMaxHealth() const {
    return maxHealth_;
}

void Health::setMaxHealth(int maxHealth) {
    maxHealth_ = maxHealth;
    clampHealth_();
}

void Health::damage(int amount, GameObject* damager) {
    if (health_ <= 0) { // if already dead, do nothing
        return;
    }

    health_ -= amount;
    if (health_ <= 0) {
        onDeath.raise({damager});
    }
    clampHealth_();
}

void Health::reset() {
    health_ = maxHealth_;
}

void Health::heal(int amount) {
    health_ += amount;
    clampHealth_();
}

void Health::heal(float amount) {
    health_ += amount;
    clampHealth_();
}

void Health::clampHealth_() {
    Mathf::clamp(health_, 0, maxHealth_);
}

Health::Health() : onDeath(this) { }

int Health::getHealth() const {
    return (int)health_;
}

int Health::getAutoHealingRate() const {
    return autoHealingRate_;
}

void Health::setAutoHealingRate(int autoHealingRate) {
    autoHealingRate_ = autoHealingRate;
}

void Health::deserialize(const json::Object &jsonObject) {
    if (jsonObject.HasKey("maxHealth")) {
        maxHealth_ = jsonObject["maxHealth"].ToInt();
    }
    if (jsonObject.HasKey("autoHealingRate")) {
        autoHealingRate_ = jsonObject["autoHealingRate"].ToInt();
    }
    reset();
}

Health::Health(Health &otherHealth) : onDeath(otherHealth.onDeath, this) {
    maxHealth_ = otherHealth.maxHealth_;
    autoHealingRate_ = otherHealth.autoHealingRate_;
    reset();
}

Tile2DComponent *Health::clone() {
    return new Health(*this);
}