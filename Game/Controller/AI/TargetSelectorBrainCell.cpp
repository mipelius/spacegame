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
#include "TargetSelectorBrainCell.h"
#include "Brains.h"
#include "Team.h"
#include "SpaceGameObject.h"
#include "CpuController.h"

TargetSelectorBrainCell::TargetSelectorBrainCell(double tickSeconds, double maxDistance): BrainCell(tickSeconds) {
    this->maxDistance = maxDistance;
}

void TargetSelectorBrainCell::operate() {
    SpaceGameObject* object = getController()->getControllableObject();

    SpaceGameObject* closestTarget = nullptr;
    double closestTargetDistance = maxDistance;

    for (std::list<Team*>::iterator it = getEnemyTeams()->begin(); it != getEnemyTeams()->end(); it++) {
        for (std::list<SpaceGameObject*>::iterator targetIt = (*it)->getMembers()->begin(); targetIt != (*it)->getMembers()->end(); targetIt++) {
            double distance = (*targetIt)->getLocation().distance(object->getLocation());
            if (distance < maxDistance) {
                if (distance < closestTargetDistance) {
                    closestTarget = (*targetIt);
                    closestTargetDistance = distance;
                }
            }
        }
    }

    this->_targetObject = closestTarget;
}

SpaceGameObject *TargetSelectorBrainCell::getTarget() {
    return this->_targetObject;
}
