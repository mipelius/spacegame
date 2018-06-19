// MIT License
//
// This file is part of SpaceGame.
// Copyright (c) 2014-2018 Miika Pelkonen
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


#ifndef __PolygonCollider_H
#define __PolygonCollider_H

#include <vector>
#include "ISerializable.h"
#include "Tile2DComponent.h"
#include "Vec.h"
#include "Rect.h"
#include "Event.h"
#include "Tile.h"
#include "ColliderLayerMatrix.h"

class Body;
class PolygonCollider;

struct CollisionEventArgs {
    PolygonCollider* const otherCollider;
    Vecf contactNormal;
};

struct TerrainCollisionEventArgs {
    float deltaTime;
    Vecf contactNormal;
    Vecf tileCoordinates;
    Tile* tile;
    Vecf velocityBeforeCollision;
};

class PolygonCollider : public Tile2DComponent, public ISerializable {
    friend class PhysicsWorld;
    friend class PolygonCollider;
    friend class Projection;

public:
    PolygonCollider();

    void setPoints(std::vector<Vecf> points);
    std::vector<Vecf> getAxes() const;

    float getSweepingStrategyThreshold() const;
    void setSweepingStrategyThreshold(float sweepingStrategyThreshold);
    const ColliderLayer& getLayer() const;
    void setLayer(const ColliderLayer& layer);

Event<PolygonCollider, CollisionEventArgs>        collision;
Event<PolygonCollider, TerrainCollisionEventArgs> terrainCollision;

    bool cast(
            const Vecf&              direction,
            const PolygonCollider&   otherCollider,
            Vecf&                    contactNormal,
            Vecf&                    toCollision
    );

    const Rect& boundingBox() const;
    const Rect boundingBoxWorldCoordinates();

    const std::vector<Vecf>& points() const;

    bool overlap(
            const PolygonCollider&   otherCollider,
            Vecf&                    contactNormal,
            float&                   penetration
    ) const;

    void deserialize(const json::Object &jsonObject) override;

protected:
    void init() override;
    void onDestroy() override;
    Tile2DComponent *clone() override;

private:
    bool detectCollisionWith_(PolygonCollider& otherCollider);
    bool detectTerrainCollision_(float deltaTime);

    Vecf pos_;
    float rot_;

    bool castOneWay_(
            const Vecf&             direction,
            const PolygonCollider&  collider,
            const PolygonCollider&  otherCollider,
            Vecf&                   contactNormal,
            Vecf&                   toCollision
    );

    float sweepingStrategyThreshold_;

    Rect boundingBox_;
    Body* body_ = nullptr;
    std::vector<Vecf> points_;
    const ColliderLayer* layer_ = nullptr;
};


#endif //__PolygonCollider_H
