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

#include "Bomb.h"


#include <iostream>
#include <math.h>
#include "Bomb.h"
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
#include "Explosion.h"
#include "Game.h"
#include "Canvas.h"

class Bomb::Body_MapCollisionEventHandler : public IEventHandler<Body, EventArgs> {
private:
    Bomb* owner;

public:
    Body_MapCollisionEventHandler(Bomb* owner) {
        this->owner = owner;
    };

    void handle(Body* body, EventArgs args) {

        new Explosion(body->location->get(), 300);

        body->location->set(body->location->get() - body->velocity->get());
        body->speed->set(Vector(0, 0));

        int blockW = body->getWorld()->getMap()->getBlockW();
        int blockH = body->getWorld()->getMap()->getBlockH();

        for (int i = -150; i < 150; i += blockW) {
            for (int j = -150; j < 150; j += blockH) {
                double distanceFromCenter = sqrt(i*i + j*j);

                if (distanceFromCenter + rand() % 15 < 150) {
                    body->getWorld()->getMap()->setValueScaled(
                            body->location->get() + Vector(i, j),
                            Block::getEmptyBlock()
                    );
                }
            }
        }

        delete owner;
    }
};

class Bomb::Body_BodyCollisionEventHandler : public IEventHandler<Body, BodyCollisionEventArgs> {
    void handle(Body* body, BodyCollisionEventArgs args) {
        //
    }
};

class Bomb::BombBody : public Body {

public:
    BombBody(double mass) : Body(mass) { }

    void afterStep() {
        double newAngle = velocity->get().angle();
        angle->set(newAngle);
    }

};

Bomb::Bomb() :
location        (   new SimpleProperty<Point>   (&location_)    ),
location_       (   Point(0, 0)                                 ),
body            (   new BombBody(200.0)                         ),
spriteContainer (   new SpriteContainer()                       )

{
    // body

    body->mapCollision->add(new Body_MapCollisionEventHandler(this));
//    body->bodyCollision->add(new Body_BodyCollisionEventHandler());

    Point points[] = {
            Point(-10, -5),
            Point(8, 0),
            Point(-10, 5),
    };

    CollisionShape* shape = new CollisionShape(points, 3);

    body->setCollisionShape(shape);

    // sprite container

    Sprite* sprite = new Sprite(
        App::getResources()->textures->bomb,
        Rect(-10, -10, 10, 10)
    );

    spriteContainer->addSprite(sprite);

    // bindings

    this->location->bind(body->location);

    spriteContainer->location->bind(body->location);
    spriteContainer->angle->bind(body->angle);
}

Bomb::~Bomb() {
    Game::getInstance()->getCanvas()->removeDrawable(spriteContainer);
    delete spriteContainer;
    body->die();
}