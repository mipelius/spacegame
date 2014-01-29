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

#include "HumanController.h"

#include "SpaceGameObject.h"
#include "Spaceship.h"

void HumanController::control(double timeElapsedSec) {
    Controller::control(timeElapsedSec);

    if (_left) handleLeft();
    if (_right) handleRight();
    if (_up) handleUp();
    if (_down) handleDown();
    if (_space) handleSpace();
    if (_enter) handleEnter();

    _setAllFalse();
}

void HumanController::left() {
    _left = true;
}

void HumanController::right() {
    _right = true;
}

void HumanController::up() {
    _up = true;
}

void HumanController::down() {
    _down = true;
}

void HumanController::space() {
    _space = true;
}

void HumanController::enter() {
    _enter = true;
}

void HumanController::_setAllFalse() {
    _left = false;
    _right = false;
    _up = false;
    _down = false;
    _space = false;
    _enter = false;
}

void HumanController::handleLeft() {
    if (objectControllable->getAngularVelocity() > -80) objectControllable->applyTorque(-359);
}

void HumanController::handleRight() {
    if (objectControllable->getAngularVelocity() < 80) objectControllable->applyTorque(359);
}

void HumanController::handleUp() {
    Spaceship* spaceship = dynamic_cast<Spaceship*>(objectControllable);
    if (spaceship) {
        if (spaceship->isStuck()) {
            spaceship->setLocation(spaceship->getLocation() + Vector::byAngle(spaceship->getAngle(), 2));
            spaceship->setSpeed(Vector::byAngle(spaceship->getAngle(), 200));
            spaceship->setNotStuck();
        }
        spaceship->accelerate();
        spaceship->applyForce(Vector::byAngle(spaceship->getAngle(), 120000));
    }
}

void HumanController::handleDown() {
    // nothing yet...
}

void HumanController::handleSpace() {
    Spaceship* spaceship = dynamic_cast<Spaceship*>(objectControllable);
    if (spaceship) {
        spaceship->shoot();
    }
}

void HumanController::handleEnter() {
    // nothing yet ...
}
