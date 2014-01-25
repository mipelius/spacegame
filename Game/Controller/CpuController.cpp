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

#include "CpuController.h"
#include "SpaceGameObject.h"
#include "Spaceship.h"

void CpuController::control() {
    Controller::control();
    if (objectControllable) {
        Spaceship* spaceship = dynamic_cast<Spaceship*>(objectControllable);

        if (spaceship) {
            if (!(rand() % 300)) spaceship->shoot();
        }

        if (enemyTarget) {
            if (enemyTarget->getLocation().distance(objectControllable->getLocation()) < 1000) {

                double deltaY = enemyTarget->getLocation().y - objectControllable->getLocation().y;
                double deltaX = enemyTarget->getLocation().x - objectControllable->getLocation().x;
                if (deltaX == 0) deltaX = 0.1;
                if (deltaY == 0) deltaY = 0.1;

                double angle = atan(deltaY / deltaX) * 180 / M_PI;
                if (deltaX < 0) angle -= 180;

                objectControllable->setAngle(angle);
                objectControllable->setSpeed(Vector::byAngle(angle, 400));

            }
        }
    }
}

void CpuController::setEnemyTarget(SpaceGameObject *enemyTarget) {
    this->enemyTarget = enemyTarget;
}
