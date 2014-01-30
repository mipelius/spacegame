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
#include "Node.h"
#include "Spaceship.h"
#include "EyeBrainCell.h"
#include "RouteGeneratorBrainCell.h"
#include "Route.h"

static const double NODE_REACHED_DISTANCE = 3.0;

NavigatorBrainCell::NavigatorBrainCell(double tickSeconds) : BrainCell(tickSeconds) {
    eye = nullptr;
    routeGenerator = nullptr;
}

void NavigatorBrainCell::operate() {
    if (!eye) return;

    Point targetLocation = eye->getTargetLocation();

    if (eye->seesTarget()) {
        routeGenerator->disable();
        navigate(targetLocation);
    }
    else {
        if (!routeGenerator) return;

        routeGenerator->enable();
        routeGenerator->setDestination(targetLocation);
        navigate(routeGenerator->getRoute());
    }
}

void NavigatorBrainCell::navigate(Point location) {
    SpaceGameObject* obj = getController()->getControllableObject();

    double deltaY = location.y - obj->getLocation().y;
    double deltaX = location.x - obj->getLocation().x;
    if (deltaX == 0) deltaX = 0.1;
    if (deltaY == 0) deltaY = 0.1;

    double angle = atan(deltaY / deltaX) * 180 / M_PI;
    if (deltaX < 0) angle -= 180;

    obj->setAngle(angle);

    obj->setSpeed(Vector::byAngle(angle, 1000.0));

    // TODO: this is just temporary stuff... remove in some point

    Spaceship* ship = dynamic_cast<Spaceship*>(obj);

    if (ship) {
        ship->accelerate();
    }
}

void NavigatorBrainCell::navigate(Route *route) {
    if (!route) return;

    SpaceGameObject* obj = getController()->getControllableObject();

    Node* node = route->getCurrentNode();

    if (node && node->getLocation().distance(obj->getLocation()) < NODE_REACHED_DISTANCE) {
        route->nextNode();
        node = route->getCurrentNode();
    }

    if (node) {
        navigate(node->getLocation());
    }
}

void NavigatorBrainCell::setRouteGeneratorBrainCell(RouteGeneratorBrainCell *routeGeneratorBrainCell) {
    this->routeGenerator = routeGeneratorBrainCell;
}

void NavigatorBrainCell::setEyeBrainCell(EyeBrainCell *eyeBrainCell) {
    this->eye = eyeBrainCell;
}
