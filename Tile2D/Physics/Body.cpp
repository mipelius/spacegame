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

#include "PhysicsWorld.h"
#include "TileMap.h"
#include "BodyCollisionEventArgs.h"
#include "PolygonCollider.h"

Body::Body() :
    // events

    bodyCollision   (   Event<Body, BodyCollisionEventArgs>(this) ),
    mapCollision    (   Event<Body, MapCollisionEventArgs>(this) ),

    // private member variables

    position_       (   Vecf(0,0) ),
    velocity_       (   Vecf(0,0) ),
    force_          (   Vecf(0,0) ),
    angularVelocity_(   0.0         ),
    mass_           (   1.0         ),

    physicsWorld_                       (nullptr),
    collider_                           (nullptr)
{

}

void Body::init() {
    position_ = {(double)transform()->position_.x, (double)transform()->position_.y};
    Tile2D::physicsWorld().add(this);
}

void Body::onDestroy() {
    if (physicsWorld_ != nullptr) {
        physicsWorld_->remove(this);
    }
}

void Body::step_(float timeElapsedSec) {
    // calculate drag

    Vecd drag = Vecd(0, 0);

    if (velocity_.x != 0 || velocity_.y != 0) {
        double velLength = velocity_.length();
        Vecd dragUnitVector = (velocity_ * -1) / velLength;
        drag = dragUnitVector * velLength * (0.5 * physicsWorld_->getAirDensity());
    }

    Vecd totalForce = force_ + drag;

    // use acceleration for updating velocity --> position

    Vecd acceleration = totalForce / mass_;
    acceleration += physicsWorld_->gForce_;
    velocity_ += acceleration;
    position_ = position_ + (velocity_ * timeElapsedSec * physicsWorld_->getMetersPerPixel());

    // apply rotation change

    float& rotation = gameObject()->transform().rotation_;
    rotation = rotation + angularVelocity_;

    // update transform position

    gameObject()->transform().position_ = {(float)position_.x, (float)position_.y};

    // remove all applied forces

    force_ = Vecd(0.0, 0.0);
}

bool Body::detectMapCollision_(float deltaTime) {
    TileMap* map = this->physicsWorld_->map_;

    if (map == nullptr || collider_ == nullptr) {
        return false;
    }

    bool collided = false;

    collider_->pos_ = {(float)position_.x, (float)position_.y};
    collider_->rot_ = gameObject()->transform().rotation_;

    Rect boundingBox = collider_->boundingBox();
    boundingBox.x1 += position_.x;
    boundingBox.x2 += position_.x;
    boundingBox.y1 += position_.y;
    boundingBox.y2 += position_.y;

    int blockSizeW = map->getTileSet()->getTileW();
    int blockSizeH = map->getTileSet()->getTileH();

    int iBegin = (int)boundingBox.x1 - ((int)boundingBox.x1) % blockSizeW;
    int iEnd = (int)boundingBox.x2 + (int)boundingBox.x2 % blockSizeW;
    int jBegin = (int)boundingBox.y1 - ((int)boundingBox.y1) % blockSizeH;
    int jEnd = (int)boundingBox.y2 + (int)boundingBox.y2 % blockSizeH;

    Vecf direction = velocity_ * deltaTime * Tile2D::physicsWorld().metersPerPixel_;
    Tile* tile;

    float w = blockSizeW;
    float h = blockSizeH;
    PolygonCollider tileCollider;
    tileCollider.rot_ = 0.0;
    tileCollider.setPoints({{0.0, 0.0}, {0.0, h}, {w, h}, {w, 0.0}});

    if (direction.length() < sqrt(blockSizeW * blockSizeH) / 4.0) { // if slowly moving object -> use SAT strategy
        Vecf contactNormal;
        float penetration;

        for (int i=iBegin; i <= iEnd; i += blockSizeW) {
            for (int j=jBegin; j <= jEnd ; j += blockSizeH) {
                tile = map->getValueScaled(Vecf(i, j));
                if (tile != nullptr && tile->getDensity() > 0.0) {
                    tileCollider.pos_ = {(float)i, (float)j};

                    if (collider_->overlap(tileCollider, contactNormal, penetration)) {
                        collider_->pos_ += contactNormal * (penetration + 0.05);
                        collided = true;

                        mapCollision.raise(MapCollisionEventArgs(deltaTime, contactNormal, tileCollider.pos_, tile));
                    }
                }
            }
        }

        if (collided) {
            velocity_ = {0, 0};
            position_ = {(double)collider_->pos_.x, (double)collider_->pos_.y};
        }

    } else { // if fast object -> use polygon casting/sweeping
        Vecf contactNormal;
        Vecf currentContactNormal;

        Vecf toCollision = {0, 0};
        Vecf currentToCollision;

        Tile* collisionTile = nullptr;
        Vecf collisionTileCoord;

        for (int i=iBegin; i <= iEnd; i += blockSizeW) {
            for (int j=jBegin; j <= jEnd ; j += blockSizeH) {
                tile = map->getValueScaled(Vecf(i, j));
                if (tile != nullptr && tile->getDensity() > 0.0) {
                    tileCollider.pos_ = {(float)i, (float)j};

                    if (collider_->cast(direction * -1.0, tileCollider, currentContactNormal, currentToCollision))
                    {
                        collided = true;

                        if (currentToCollision.lengthSqr() > toCollision.lengthSqr()) {
                            collisionTile = tile;
                            collisionTileCoord = tileCollider.pos_;

                            toCollision = currentToCollision;
                            contactNormal = currentContactNormal;
                        }
                    }
                }
            }
        }

        if (collided) {
            const Vecd v = velocity_;
            const Vecf& n = contactNormal;

            Vecf proj_n_v = n * v.dot(n);
            Vecd reflVel = v + proj_n_v * -2.0;

            velocity_ = reflVel * 0.2;
            position_ += toCollision * (1.01);

            mapCollision.raise(MapCollisionEventArgs(deltaTime, n, collisionTileCoord, collisionTile));
        }
    }

    transform()->position_ = {(float)position_.x, (float)position_.y};

    return collided;
}


bool Body::detectCollisionWith_(Body &otherBody) {
    if (collider_ == nullptr || otherBody.collider_ == nullptr) return false;
    if (!collider_->boundingBox_.intersectsWith(otherBody.collider_->boundingBox_)) return false;

    Vecf contactNormal;
    float penetration;

    if (collider_->overlap(*otherBody.collider_, contactNormal, penetration)) {
        bodyCollision.raise(BodyCollisionEventArgs(&otherBody, contactNormal));
        return true;
    }

    return false;
}

void Body::applyForce(Vecd force) {
    force_ += force;
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

// getters and setters

float Body::getMass() const {
    return mass_;
}

void Body::setMass(float mass) {
    mass_ = mass;
}

float Body::getAngularVelocity() const {
    return angularVelocity_;
}

void Body::setAngularVelocity(float angularVelocity) {
    angularVelocity_ = angularVelocity;
}

const Vecd &Body::getVelocity() const {
    return velocity_;
}

void Body::setVelocity(const Vecd &velocity) {
    velocity_ = velocity;
}