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

#include <iostream>
#include "MyGameObject.h"
#include "IEventHandler.h"
#include "EventArgs.h"
#include "Event.h"
#include "CollisionShape.h"
#include "SpriteContainer.h"
#include "Texture.h"
#include "Sprite.h"
#include "SimpleProperty.h"

class MyGameObject::Body_MapCollisionEventHandler : public IEventHandler<Body, EventArgs> {
    void handle(Body* body, EventArgs args) {
        body->location->set(body->location->get() - body->velocity->get());
        body->speed->set(Vector(0, 0));
    }
};

static Texture* texture = nullptr;

MyGameObject::MyGameObject() :
    location        (   new SimpleProperty<Point>   (&location_)    ),
    location_       (   Point(0, 0)                                 ),
    body            (   new Body(100)                               ),
    spriteContainer (   new SpriteContainer()                       )

{
    // body

    body->mapCollision->add(new Body_MapCollisionEventHandler());

    Point points[] = {
        Point(-10, -10),
        Point(10, -10),
        Point(10, 10),
        Point(-10, 10),
    };

    CollisionShape* shape = new CollisionShape(points, 3);

    body->setCollisionShape(shape);

    // sprite container

    if (!texture) texture = new Texture("images/spaceship.png");

    Sprite* sprite = new Sprite(texture, Rect(-20, -20, 20, 20));

    spriteContainer->addSprite(sprite);

    // bindings

    this->location->bind(body->location);

    spriteContainer->location->bind(body->location);
    spriteContainer->angle->bind(body->angle);
}

MyGameObject::~MyGameObject() {
    delete body;
}
