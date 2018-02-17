// This file is part of SpaceGame.
// Copyright (C) 2018 Miika Pelkonen
//
// SpaceGame is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// SpaceGame is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SpaceGame.  If not, see <http://www.gnu.org/licenses/>.


#include "Health.h"
#include "Tile2DMath.h"

void Health::awake() {
    health_ = maxHealth_;
}

void Health::update() { }

void Health::lateUpdate() { }

int Health::getMaxHealth() const {
    return maxHealth_;
}

void Health::setMaxHealth(int maxHealth) {
    maxHealth_ = maxHealth;
    clampHealth_();
}

void Health::damage(int amount, GameObject* damager) {
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

void Health::clampHealth_() {
    Mathi::clamp(health_, 0, maxHealth_);
}

Health::Health() : onDeath(this) {

}

int Health::getHealth() const {
    return health_;
}
