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

#include "BrainCell.h"
#include "CpuController.h"
#include "Brains.h"
#include "SpaceGameObject.h"
#include <list>

BrainCell::BrainCell(double tickSeconds) {
    timerSeconds = 0;
    this->tickSeconds = tickSeconds;
}

void BrainCell::operate(double timeElapsedSeconds) {
    timerSeconds += timeElapsedSeconds;
    if (timerSeconds >= tickSeconds) {
        operate();
        timerSeconds = 0;
    }
}

void BrainCell::operate() {

}

std::list<Team*> *BrainCell::getEnemyTeams() {
    return _brains->enemyTeams;
}

CpuController *BrainCell::getController() {
    return _brains->controller;
}

void BrainCell::setTarget(SpaceGameObject *target) {
    _brains->target = target;
}

SpaceGameObject *BrainCell::getTarget() {
    return _brains->target;
}

void BrainCell::setRouteNextNode(Node *node) {
    this->_brains->routeNextNode = node;
}

Node* BrainCell::getRouteNextNode() {
    return this->_brains->routeNextNode;
}
