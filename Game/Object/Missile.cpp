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

#include <math.h>
#include "Missile.h"
#include "Body.h"
#include "IEventHandler.h"
#include "EventArgs.h"
#include "BodyCollisionEventArgs.h"
#include "Texture.h"
#include "Event.h"
#include "Sprite.h"
#include "SpriteContainer.h"
#include "CollisionShape.h"
#include "Game.h"
#include "Canvas.h"
#include "PhysicsWorld.h"
#include "Explosion.h"
#include "Block.h"
#include "WorldMap.h"
#include "PulseLight.h"
#include "Sample.h"
#include "App.h"
#include "SamplePlayer.h"

Sample* Missile::missileCollisionSample_ = nullptr;
int Missile::sampleChannel_;

class Missile::MissileBody : public Body {

private:
    Point initialLocation_;
    Missile* owner_;

public:
    MissileBody(double mass, Point initialLocation, Missile* owner) : Body(mass), initialLocation_(initialLocation) {
        location->set(initialLocation_);
        owner_ = owner;
    }

    void afterStep() {
        if (initialLocation_.distance(location->get()) > 1000) {
            die();
        }
    }

    void onDie() {
        delete owner_;
    }
};

class Missile::Body_MapCollisionEventHandler : public IEventHandler<Body, EventArgs> {
    void handle(Body* body, EventArgs args) {
        new PulseLight(body->location->get(), 100.0);

        Point* point = new Point(0, 0);
        point->x = body->location->get().x;
        point->y = body->location->get().y;
        App::getInstance()->getSamplePlayer()->play(missileCollisionSample_, sampleChannel_, point);

        sampleChannel_++;

        if (sampleChannel_ > 7) {
            sampleChannel_ = 0;
        }

        for (int i = -15; i < 15; i++) {
            for (int j = -15; j < 15; j++) {
                double distanceFromCenter = sqrt(i*i + j*j);

                if (distanceFromCenter < 15) {
                    body->getWorld()->getMap()->setValueScaled(
                            body->location->get() + Vector(i, j),
                            Block::getEmptyBlock()
                    );
                }
            }
        }

        body->die();
    }
};

class Missile::Body_BodyCollisionEventHandler : public IEventHandler<Body, BodyCollisionEventArgs> {
    void handle(Body* body, BodyCollisionEventArgs args) {
//        body->location->set(body->location->get() - body->velocity->get());
//        body->speed->set(Vector(0, 0));
    }
};

static Texture* texture = nullptr;

static Texture* accelerationAnimationTexture = nullptr;

Missile::Missile(Point initialLocation, Vector force) :
body            (   new MissileBody(10.0, initialLocation, this)    ),
spriteContainer (   new SpriteContainer()                           )

{
    if (!missileCollisionSample_) {
        sampleChannel_ = 0;
        missileCollisionSample_ = new Sample("soundfx/missile_collision.wav", 10);
    }

    // body

    body->mapCollision->add(new Body_MapCollisionEventHandler());
    body->bodyCollision->add(new Body_BodyCollisionEventHandler());

    body->angle->set(force.angle());

    Point points[] = {
            Point(-10, -3),
            Point(10, 0),
            Point(-10, 3),
    };

    CollisionShape* shape = new CollisionShape(points, 3);

    body->setCollisionShape(shape);
    Game::getInstance()->getWorld()->add(body);

    body->force->set(force);

    // load textures

    if (!texture) {
        texture = new Texture("images/missile.png");
    }

    // sprite container

    Sprite* sprite = new Sprite(texture, Rect(-10, -5, 10, 5));
    spriteContainer->addSprite(sprite);
    Game::getInstance()->getExternalCanvas()->addDrawable(spriteContainer);

    // bindings

    spriteContainer->location->bind(body->location);
    spriteContainer->angle->bind(body->angle);
}

Missile::~Missile() {
    Game::getInstance()->getExternalCanvas()->removeDrawable(spriteContainer);
    delete spriteContainer;

    delete body;
}