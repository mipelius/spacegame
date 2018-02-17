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

#include "HUD.h"

void HUD::awake() {
    health_ = gameObject()->getComponent<Health>();
}

void HUD::update() {
    Rect rect = healthSprite_->getRect();
    rect.x2 = (float)health_->getHealth();
    healthSprite_->setRect(rect);
}

void HUD::lateUpdate() {

}

void HUD::setHealthSprite(Sprite *healthSprite) {
    healthSprite_ = healthSprite;
}

void HUD::hide() {
    healthSprite_->setIsVisible(false);
}

void HUD::show() {
    healthSprite_->setIsVisible(true);
}
