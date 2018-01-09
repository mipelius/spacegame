//
// Created by Miika Pelkonen on 15/12/2017.
//

#ifndef __PolygonCollider_H
#define __PolygonCollider_H

#include <vector>
#include "Vec.h"
#include "Rect.h"

class PolygonCollider {
    friend class PolygonCollider;
public:
    explicit PolygonCollider(std::vector<Vec> points);

    bool cast(
            const Vec&              direction,
            const PolygonCollider&  otherCollider,
            Vec&                    contactNormal,
            Vec&                    toCollision
    );

    Vec pos;
    double rot;

    const Rect& boundingBox() const;

    std::vector<Vec> points;

    bool overlap(const PolygonCollider &otherCollider, Vec& contactNormal, double& penetration) const;

    std::vector<Vec> getAxes() const;

private:
    bool castOneWay_(
            const Vec &direction,
            const PolygonCollider &collider,
            const PolygonCollider &otherCollider,
            Vec &contactNormal,
            Vec &toCollision
    );

    Rect boundingBox_;

};

#endif //__PolygonCollider_H
