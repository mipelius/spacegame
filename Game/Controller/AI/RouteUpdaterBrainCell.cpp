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
#include "RouteUpdaterBrainCell.h"

#include "RouteUpdaterRequest.h"
#include "RouteResponse.h"
#include "RouteRequest.h"
#include "SpaceGameObject.h"
#include "GameWorld.h"
#include "RouteGenerator.h"
#include "CpuController.h"
#include "CollisionShape.h"

RouteUpdaterBrainCell::RouteUpdaterBrainCell(double tickSeconds):BrainCell(tickSeconds) {

}

void RouteUpdaterBrainCell::handleResponse(RouteResponse *response) {
    switch (response->getMsg()) {
        case(RouteResponse::RouteResponseMessage::ROUTE_FOUND):
            setRouteNextNode(response->getFirstNode());
            break;

        case(RouteResponse::RouteResponseMessage::ROUTE_NOT_FOUND):
            setRouteNextNode(nullptr);
            break;

        case(RouteResponse::RouteResponseMessage::ROUTE_TIME_OUT):
            break;
    }
}

void RouteUpdaterBrainCell::operate() {
    SpaceGameObject* target = getTarget();

    if (target && !target->isDead()) {
        target->getWorld()->getRouteGenerator()->sendRequest(
            new RouteUpdaterRequest(
                getController()->getControllableObject()->getLocation(),
                target->getLocation(),
                2,
                getController()->getControllableObject()->getCollisionShape()->getBoundingBox(),
                this
            )
        );
    }

    else setRouteNextNode(nullptr);
}
