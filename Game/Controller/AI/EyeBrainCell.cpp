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
#include "EyeBrainCell.h"
#include "TargetSelectorBrainCell.h"
#include "SpaceGameObject.h"
#include "CpuController.h"
#include "GameWorld.h"
#include "Map.h"

bool EyeBrainCell::seesTarget() {
    return _targetIsInSight;
}

EyeBrainCell::EyeBrainCell(double tickSeconds) : BrainCell(tickSeconds), _targetLocation(Point(0, 0)) {
    _targetIsInSight = false;
}

Point EyeBrainCell::getTargetLocation() {
    return _targetLocation;
}

void EyeBrainCell::operate() {
    if (!_targetSelectorBrainCell) return;

    SpaceGameObject* target = _targetSelectorBrainCell->getTarget();

    if (target) {
        this->_targetLocation = target->getLocation();
        updateIsInSight();
    }
    else {
        _targetIsInSight = false;
    }
}

void EyeBrainCell::setTargetSelectorBrainCell(TargetSelectorBrainCell *targetSelectorBrainCell) {
    this->_targetSelectorBrainCell = targetSelectorBrainCell;
}

void EyeBrainCell::updateIsInSight() {
    SpaceGameObject* obj = getController()->getControllableObject();

    if (obj) {
        _targetIsInSight = true;

        double deltaX = _targetLocation.x - obj->getLocation().x;
        double deltaY = _targetLocation.y - obj->getLocation().y;

        double divider = fabs((fabs(deltaX) > fabs(deltaY) ? deltaX : deltaY));

        double xStep = deltaX / divider;
        double yStep = deltaY / divider;

        double x = obj->getLocation().x;
        double y = obj->getLocation().y;

        bool xIsReady = false;
        bool yIsReady = false;

        while (!(xIsReady && yIsReady)) {
            x += xStep;
            y += yStep;

            if (obj->getWorld()->getMap()->getValueActual((int)x, (int)y)) {
                _targetIsInSight = false;
                return;
            }

            xIsReady = (deltaX < 0 ? x <= _targetLocation.x : x >= _targetLocation.x);
            yIsReady = (deltaY < 0 ? y <= _targetLocation.y : y >= _targetLocation.y);
        }
    }
}
