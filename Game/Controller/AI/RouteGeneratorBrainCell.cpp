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
#include "RouteGeneratorBrainCell.h"

#include "RouteResponse.h"
#include "RouteRequest.h"
#include "SpaceGameObject.h"
#include "GameWorld.h"
#include "RouteGenerator.h"
#include "CpuController.h"
#include "CollisionShape.h"
#include "Route.h"
#include "BrainCell.h"

static unsigned int ROUTE_GENERATOR_STEP = 3;

RouteGeneratorBrainCell::RouteGeneratorBrainCell(double tickSeconds):BrainCell(tickSeconds), _destination(Point(0, 0)) {
    this->_route = nullptr;
}

void RouteGeneratorBrainCell::handleResponse(RouteResponse *response) {
    switch (response->getMsg()) {
        case(RouteResponse::Message::ROUTE_FOUND):
            removeRoute();
            this->_route = response->getRoute();
            this->_route->begin();
            break;

        case(RouteResponse::Message::ROUTE_NOT_FOUND):
            removeRoute();
            break;

        case(RouteResponse::Message::ROUTE_TIME_OUT):
            break;
    }
}

void RouteGeneratorBrainCell::operate() {
    // if not enabled, delete existing route, and return

    if (!_isEnabled) return;

    SpaceGameObject* controllableObject = this->getController()->getControllableObject();

    if (controllableObject) {
        this->setRecipient(controllableObject->getWorld()->getRouteGenerator());

        sendRequest(
                new RouteRequest(
                        _destination,
                        ROUTE_GENERATOR_STEP,
                        getController()->getControllableObject()->getCollisionShape()->getBoundingBox()
                )
        );
    }
}

Point RouteGeneratorBrainCell::getLocation() {
    return this->getController()->getControllableObject()->getLocation();
}

void RouteGeneratorBrainCell::enable() {
    if (!_isEnabled) {
        this->timerSeconds = this->tickSeconds;
    }

    this->_isEnabled = true;
}

void RouteGeneratorBrainCell::disable() {
    removeRoute();
    this->_isEnabled = false;
}

bool RouteGeneratorBrainCell::isEnabled() {
    return _isEnabled;
}

Route *RouteGeneratorBrainCell::getRoute() {
    return this->_route;
}

void RouteGeneratorBrainCell::setDestination(Point location) {
    _destination = location;
}

void RouteGeneratorBrainCell::removeRoute() {
    if (_route) {
        delete _route;
        _route = nullptr;
    }
}
