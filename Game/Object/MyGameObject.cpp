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
#include "Body.h"
#include "BodyCollisionEventArgs.h"
#include "AnimatedTexture.h"
#include "App.h"
#include "AnimationManager.h"
#include "PhysicsWorld.h"
#include "WorldMap.h"

class MyGameObject::Body_MapCollisionEventHandler : public IEventHandler<Body, EventArgs> {
    void handle(Body* body, EventArgs args) {

        body->location->set(body->location->get() - body->velocity->get());
        body->speed->set(Vector(0, 0));

    }
};

class MyGameObject::Body_BodyCollisionEventHandler : public IEventHandler<Body, BodyCollisionEventArgs> {
    void handle(Body* body, BodyCollisionEventArgs args) {
//        body->location->set(body->location->get() - body->velocity->get());
//        body->speed->set(Vector(0, 0));
    }
};

static Texture* texture = nullptr;

static Texture* accelerationAnimationTexture = nullptr;

MyGameObject::MyGameObject() :
    location        (   new SimpleProperty<Point>   (&location_)    ),
    location_       (   Point(0, 0)                                 ),
    body            (   new Body(100.0)                             ),
    spriteContainer (   new SpriteContainer()                       )

{
    // body

    body->mapCollision->add(new Body_MapCollisionEventHandler());
    body->bodyCollision->add(new Body_BodyCollisionEventHandler());

    Point points[] = {
        Point(-20, -20),
        Point(20, 0),
        Point(-20, 20),
    };

    CollisionShape* shape = new CollisionShape(points, 3);

    body->setCollisionShape(shape);

    // load textures

    if (!texture) {
        texture = new Texture("images/spaceship.png");
    }

    if (!accelerationAnimationTexture) {
        accelerationAnimationTexture = new Texture("images/anim_rocket_fire.png");
    }

    // sprite container

    Sprite* sprite = new Sprite(texture, Rect(-20, -20, 20, 20));
    accelerationAnimation_ = new AnimatedTexture(60, 8, false, accelerationAnimationTexture);
    App::getInstance()->getAnimationManager()->add(accelerationAnimation_);

    Sprite* spriteAccelerationLeft = new Sprite(accelerationAnimation_, Rect(-40, -23, -20, -10));
    Sprite* spriteAccelerationRight = new Sprite(accelerationAnimation_, Rect(-40, 10, -20, 23));

    spriteContainer->addSprite(spriteAccelerationLeft);
    spriteContainer->addSprite(spriteAccelerationRight);

    spriteContainer->addSprite(sprite);

    // bindings

    this->location->bind(body->location);

    spriteContainer->location->bind(body->location);
    spriteContainer->angle->bind(body->angle);
}

MyGameObject::~MyGameObject() {
    delete body;
    delete spriteContainer;
}

void MyGameObject::accelerate() {
    double angle = body->angle->get();
    body->applyForce(Vector::byAngle(angle, 5000));
    accelerationAnimation_->play();
}
