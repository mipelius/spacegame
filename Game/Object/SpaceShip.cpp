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

#include "Spaceship.h"
#include "CollisionShape.h"
#include "BodyCollisionEventArgs.h"
#include "App.h"
#include "AnimationManager.h"
#include "PhysicsWorld.h"
#include "WorldMap.h"
#include "Missile.h"
#include "SamplePlayer.h"

class Spaceship::Body_MapCollisionEventHandler : public IEventHandler<Body, EventArgs> {
    void handle(Body* body, EventArgs args) override {
        body->position.set(body->position.get() - body->velocity.get());
        body->speed.set(Vector(0, 0));
    }
};

class Spaceship::Body_BodyCollisionEventHandler : public IEventHandler<Body, BodyCollisionEventArgs> {
    void handle(Body* body, BodyCollisionEventArgs args) override {
        //body->location.set(body->location.get() - body->velocity.get());
        //body->speed.set(Vector(0, 0));
    }
};

Spaceship::Spaceship() :
    body                    (   new Body(100.0)                                                                 ),
    spriteContainer         (   new DrawableGroup()                                                             ),
    accelerationAnimation_  (   AnimatedTexture(60, 8, false, App::getResources()->textures->animRocketFire)    )
{
    // body

    body->mapCollision.add(new Body_MapCollisionEventHandler());
    body->bodyCollision.add(new Body_BodyCollisionEventHandler());

    Point points[] = {
        Point(-20, -20),
        Point(20, 0),
        Point(-20, 20),
    };

    auto shape = new CollisionShape(points, 3);

    body->setCollisionShape(shape);

    // sprite container

    Sprite* sprite = new Sprite(
            App::getResources()->textures->spaceship,
            Rect(-20, -20, 20, 20)
    );

    App::getAnimationManager()->add(accelerationAnimation_);

    Sprite* spriteAccelerationLeft = new Sprite(&accelerationAnimation_, Rect(-40, -23, -20, -10));
    Sprite* spriteAccelerationRight = new Sprite(&accelerationAnimation_, Rect(-40, 10, -20, 23));

    spriteContainer->addDrawable(spriteAccelerationLeft);
    spriteContainer->addDrawable(spriteAccelerationRight);

    spriteContainer->addDrawable(sprite);

    // bindings

    //this->location.bind(body->location);

    spriteContainer->position.bind(body->position);
    spriteContainer->angle.bind(body->angle);
}

Spaceship::~Spaceship() {
    delete body;
    delete spriteContainer;
}

void Spaceship::accelerate() {
    double angle = body->angle.get();
    body->applyForce(Vector::byAngle(angle, 5000));
    accelerationAnimation_.play();
}

void Spaceship::shoot() {
    Missile* missileMiddle = new Missile(
            body->position.get(),
            Vector::byAngle(body->angle.get(), 100000)
    );

    Missile* missileLeft = new Missile(
            body->position.get() + Vector::byAngle(body->angle.get() - 120, 15),
            Vector::byAngle(body->angle.get(), 100000)
    );

    Missile* missileRight = new Missile(
            body->position.get() + Vector::byAngle(body->angle.get() + 120, 15),
            Vector::byAngle(body->angle.get(), 100000)
    );

    App::getSamplePlayer()->play(App::getResources()->samples->spaceshipShoot);
}
