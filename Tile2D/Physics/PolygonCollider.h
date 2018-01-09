//
// Created by Miika Pelkonen on 15/12/2017.
//

#ifndef __PolygonCollider_H
#define __PolygonCollider_H

#include <vector>
#include "Tile2DObject.h"
#include "Vec.h"
#include "Rect.h"

class PolygonCollider : public Tile2DObject {
    friend class PolygonCollider;
    friend class Body;
    friend class Projection;

public:
    PolygonCollider();

    bool cast(
            const Vec&              direction,
            const PolygonCollider&  otherCollider,
            Vec&                    contactNormal,
            Vec&                    toCollision
    );

    const Rect& boundingBox() const;
    const std::vector<Vec>& points() const;

    bool overlap(const PolygonCollider &otherCollider, Vec& contactNormal, double& penetration) const;

    void setPoints(std::vector<Vec> points);
    std::vector<Vec> getAxes() const;

private:
    Vec pos_;
    double rot_;

    bool castOneWay_(
            const Vec &direction,
            const PolygonCollider &collider,
            const PolygonCollider &otherCollider,
            Vec &contactNormal,
            Vec &toCollision
    );

    Rect boundingBox_;

    std::vector<Vec> points_;
};

#endif //__PolygonCollider_H
