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
#include "Brains.h"
#include "BrainCell.h"
#include "Team.h"
#include "SpaceGameObject.h"
#include "CpuController.h"

Brains::Brains() {
    this->enemyTeams = new std::list<Team*>();
    this->brainCells = new std::list<BrainCell*>();
    this->target = nullptr;
    this->routeNextNode = nullptr;
}

void Brains::operate(double timeSec) {
    controller->getControllableObject()->setSpeed(Vector(0, 0));
    for (std::list<BrainCell*>::iterator it = brainCells->begin(); it != brainCells->end(); it++) {
        (*it)->operate(timeSec);
    }
}

void Brains::addEnemyTeam(Team *team) {
    enemyTeams->push_back(team);
}

void Brains::addCell(BrainCell *brainCell) {
    this->brainCells->push_back(brainCell);
    brainCell->_brains = this;
}
