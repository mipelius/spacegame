// This file is part of SpaceGame.
// Copyright (C) 2014  Miika Pelkonen
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

#include "Tile2D.h"
#include "precompile.h"
#include "Body.h"

#include "ColliderShape.h"
#include "PhysicsWorld.h"
#include "TileMap.h"
#include "BodyCollisionEventArgs.h"
#include "PolygonCollider.h"

Body::Body() :
    // properties

    mass            (   Property<double>  (&mass_             )   ),

    angle           (   Property<double>  (&angle_            )   ),
    angularVelocity (   Property<double>  (&angularVelocity_  )   ),

    position        (   Property<Vec>  (&position_         )   ),
    velocity        (   Property<Vec>  (&velocity_         )   ),
    force           (   Property<Vec>  (&force_            )   ),

    // events

    bodyCollision   (   Event<Body, BodyCollisionEventArgs>(this) ),
    mapCollision    (   Event<Body, MapCollisionEventArgs>(this) ),

    // private member variables

    position_       (   Vec(0,0) ),
    velocity_       (   Vec(0,0) ),
    force_          (   Vec(0,0) ),
    angle_          (   0.0         ),
    angularVelocity_(   0.0         ),
    mass_           (   1.0         ),

    physicsWorld_                       (nullptr),
    collider_                           (nullptr)
{

}

void Body::init() {
    Tile2D::physicsWorld().add(this);
}

void Body::onDestroy() {
    collider_->destroy();
}

void Body::step_(double timeElapsedSec) {
    // calculate drag

    Vec drag = Vec(0, 0);

    if (velocity_.x != 0 || velocity_.y != 0) {
        double speedLengthPow2 = velocity_.x * velocity_.x + velocity_.y * velocity_.y;
        Vec dragUnitVector = (velocity_ * -1) * (1 / sqrt(velocity_.x * velocity_.x + velocity_.y * velocity_.y));
        drag = dragUnitVector * speedLengthPow2 * (0.5 * physicsWorld_->airDensity.get());
    }

    Vec totalForce = force_ + drag;

    // use acceleration for updating velocity --> position

    Vec acceleration = totalForce / mass_;
    acceleration += physicsWorld_->gForce_;
    velocity_ += acceleration;
    position_ = position_ + (velocity_ * timeElapsedSec * physicsWorld_->metersPerPixel.get());

    // apply angle change

    angle_ = angle_ + angularVelocity_;

    // remove all applied forces

    force_ = Vec(0, 0);

    // update dependent properties

    this->angle.updateDependentProperties();
    this->angularVelocity.updateDependentProperties();
    this->position.updateDependentProperties();
    this->velocity.updateDependentProperties();
    this->force.updateDependentProperties();
}

bool Body::detectMapCollision_(double deltaTime) {
    TileMap* map = this->physicsWorld_->map_;

    if (map == nullptr || collider_ == nullptr) {
        return false;
    }

    bool collided = false;

    collider_->pos_ = position_;
    collider_->rot_ = angle_;

    Rect boundingBox = collider_->boundingBox();
    boundingBox.x1 += position_.x;
    boundingBox.x2 += position_.x;
    boundingBox.y1 += position_.y;
    boundingBox.y2 += position_.y;

    int blockSizeW = map->getBlockW();
    int blockSizeH = map->getBlockH();

    int iBegin = (int)boundingBox.x1 - ((int)boundingBox.x1) % blockSizeW;
    int iEnd = (int)boundingBox.x2 + (int)boundingBox.x2 % blockSizeW;
    int jBegin = (int)boundingBox.y1 - ((int)boundingBox.y1) % blockSizeH;
    int jEnd = (int)boundingBox.y2 + (int)boundingBox.y2 % blockSizeH;

    Vec direction = velocity_ * deltaTime * Tile2D::physicsWorld().metersPerPixel_;
    Tile* tile;

    if (direction.length() < sqrt(blockSizeW * blockSizeH) / 4.0) {
        Vec contactNormal;
        double penetration;

        for (int i=iBegin; i <= iEnd; i += blockSizeW) {
            for (int j=jBegin; j <= jEnd ; j += blockSizeH) {
                tile = map->getValueScaled(Vec(i, j));
                if (tile != nullptr && tile->density.get() > 0.0) {
                    double x = i;
                    double y = j;
                    double w = blockSizeW;
                    double h = blockSizeH;

                    PolygonCollider tileCollider;
                    tileCollider.setPoints({{0.0, 0.0}, {0.0, h}, {w, h}, {w, 0.0}});
                    tileCollider.rot_ = 0.0;
                    tileCollider.pos_ = {x, y};

                    if (collider_->overlap(tileCollider, contactNormal, penetration)) {
                        collider_->pos_ += contactNormal * (penetration + 0.05);
                        collided = true;
                    }
                }
            }
        }

        if (collided) {
            velocity_ = {0, 0};
            position_ = collider_->pos_;
        }

    } else {
        Vec contactNormal;
        Vec currentContactNormal;

        Vec toCollision = {0, 0};
        Vec currentToCollision;

        for (int i=iBegin; i <= iEnd; i += blockSizeW) {
            for (int j=jBegin; j <= jEnd ; j += blockSizeH) {
                tile = map->getValueScaled(Vec(i, j));
                if (tile != nullptr && tile->density.get() > 0.0) {
                    double x = i;
                    double y = j;
                    double w = blockSizeW;
                    double h = blockSizeH;

                    PolygonCollider tileCollider;
                    tileCollider.setPoints({{0.0, 0.0}, {0.0, h}, {w, h}, {w, 0.0}});
                    tileCollider.rot_ = 0.0;
                    tileCollider.pos_ = {x, y};

                    if (collider_->cast(direction * -1.0, tileCollider, currentContactNormal, currentToCollision))
                    {
                        collided = true;

                        if (currentToCollision.lengthSqr() > toCollision.lengthSqr()) {
                            toCollision = currentToCollision;
                            contactNormal = currentContactNormal;
                        }
                    }

                }
            }
        }

        if (collided) {
            const Vec& v = velocity_;
            const Vec n = contactNormal.normalized();

            Vec proj_n_v = n * v.dot(n);;
            Vec reflVel = v + proj_n_v * -2.0;

            velocity_ = reflVel * 0.2;
            position_ += toCollision * (1.01);
        }
    }

    if (collided) {
        velocity.updateDependentProperties();
        position.updateDependentProperties();
        mapCollision.raise(MapCollisionEventArgs(deltaTime));
    }

    return collided;
}


bool Body::detectCollisionWith_(Body &otherBody) {
//    if (!entityCollisionDetectionIsIgnored_) {
//        if (this->collider == nullptr || otherBody.collider == nullptr) return false;
//        if (this->collider->intersectsWith(otherBody.getColliderShape()) ||
//                otherBody.getColliderShape()->intersectsWith(collider)) {
//
//            bodyCollision.raise(BodyCollisionEventArgs(&otherBody));
//
//            return true;
//        }
//    }

    return false;
}

void Body::applyForce(Vec force) {
    Vec forceBefore = this->force.get();
    this->force.set(forceBefore + force);
}

void Body::setWorld_(PhysicsWorld *gameWorld) {
    this->physicsWorld_ = gameWorld;
}

PhysicsWorld *Body::getWorld() {
    return this->physicsWorld_;
}

PolygonCollider* Body::getCollider() {
    return collider_;
}

void Body::setCollider(PolygonCollider *collider) {
    this->collider_ = collider;
}

Body::~Body() {
    Tile2D::physicsWorld().remove(this);
}
