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

#include "DynamicLightObject.h"
#include "Body.h"
#include "PointLight.h"
#include "SpriteContainer.h"
#include "IEventHandler.h"
#include "EventArgs.h"
#include "Event.h"
#include "CollisionShape.h"
#include "PhysicsWorld.h"
#include "Map"
#include "WorldMap.h"

class DynamicLightObject::Body_MapCollisionEventHandler : public IEventHandler<Body, EventArgs> {
    void handle(Body* body, EventArgs args) {
        WorldMap * map = body->getWorld()->getMap();

//        for (int i=-2; i <= 2; i++) {
//            for (int j=-2; j <= 2; j++) {
//                int x = i*10 + body->location->get().x;
//                int y = j*10 + body->location->get().y;
//                map->setByteValueActual(x, y, 0);
//            }
//        }

        if (body->speed->get().length() < 100.0) {
            body->speed->set(Vector(0, 0));
            body->location->set(body->location->get() - body->velocity->get());
        }
        else {
            body->speed->set(body->speed->get() * 0.5);
        }
    }
};

DynamicLightObject::DynamicLightObject(Point const &location, double radius) :
    LightObject(location, radius),
    body(new Body(10.0))
{
    this->pointLight->location->bind(body->location);
    this->spriteContainer->location->bind(body->location);
    body->location->set(location);
    body->mapCollision->add(new DynamicLightObject::Body_MapCollisionEventHandler());

    Point points[] = {
            Point(-2, -2),
            Point(2, -2),
            Point(2, 2),
            Point(-2, 2),
    };

    CollisionShape* shape = new CollisionShape(points, 4);

    body->setCollisionShape(shape);
}
