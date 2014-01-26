// This file is part of SpaceGame.
// Copyright (C) 2014 Miika Pelkonen
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

#include "precompile.h"
#include "NavigatorBrainCell.h"
#include "Brains.h"
#include "CpuController.h"
#include "SpaceGameObject.h"

NavigatorBrainCell::NavigatorBrainCell(double tickSeconds, Brains* brains) : BrainCell(tickSeconds, brains) {

}

void NavigatorBrainCell::operate() {
    BrainCell::operate();
    SpaceGameObject* obj = getController()->getControllableObject();

    SpaceGameObject* target = getTarget();

    if (target) {
        double deltaY = target->getLocation().y - obj->getLocation().y;
        double deltaX = target->getLocation().x - obj->getLocation().x;
        if (deltaX == 0) deltaX = 0.1;
        if (deltaY == 0) deltaY = 0.1;

        double angle = atan(deltaY / deltaX) * 180 / M_PI;
        if (deltaX < 0) angle -= 180;

        obj->setAngle(angle);
        obj->setSpeed(Vector(0, 0));
        //obj->setSpeed(Vector::byAngle(angle, 400));
    }
}