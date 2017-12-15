//
// Created by Miika Pelkonen on 15/12/2017.
//

#ifndef __PolygonCollider_H
#define __PolygonCollider_H

#include <vector>
#include "Vec.h"
#include "Manifold.h"
#include "Rect.h"

class PolygonCollider {
    friend class PolygonCollider;
public:
    explicit PolygonCollider(std::vector<Vec> points);

    bool cast(
            const Vec&              direction,
            const PolygonCollider&  otherCollider,
            Manifold&               manifold
    );

    Vec pos;
    double rot;

    std::vector<Vec> points() const;

    const Rect& boundingBox() const;

private:
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
