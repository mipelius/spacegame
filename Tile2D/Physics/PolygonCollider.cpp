//
// Created by Miika Pelkonen on 15/12/2017.
//

#include <iostream>
#include "PolygonCollider.h"
#include "LineSegment.h"

bool PolygonCollider::castOneWay_(
        const Vec &direction,
        const PolygonCollider &collider,
        const PolygonCollider &otherCollider,
        Vec &contactNormal,
        Vec &penetration
) {
    bool collided = false;

    penetration = {10000.0, 10000.0};

    const PolygonCollider& c1 = collider;
    const PolygonCollider& c2 = otherCollider;

    const Vec& c1pos = collider.pos;
    const double& c1rot = collider.rot;
    const Vec& c2pos = otherCollider.pos;
    const double& c2rot = otherCollider.rot;

    Vec intersectionPoint;
    double minDistanceSqr = 100000000000.0f;
    double curDistanceSqr;
    Vec curVec;

    for (const auto& pointOrig : c1.points_) {
        Vec castingPoint = pointOrig.rotated(c1rot) + c1pos;
        LineSegment castingLine(castingPoint, castingPoint + direction);

        for (auto i = 0u; i < otherCollider.points_.size(); i++) {
            Vec startPoint = c2.points_[i].rotated(c2rot) + c2pos;
            Vec endPoint = c2.points_[(i+1) % c2.points_.size()].rotated(c2rot) + c2pos;
            LineSegment polygonLine(startPoint, endPoint);

            if (castingLine.intersection(polygonLine, intersectionPoint)) {
                collided = true;
                curVec = (intersectionPoint - castingPoint);
                curDistanceSqr = curVec.lengthSqr();
                if (minDistanceSqr > curDistanceSqr) {
                    minDistanceSqr = curDistanceSqr;
                    penetration = curVec;
                    Vec tmp = startPoint - endPoint;
                    contactNormal = {tmp.y, -tmp.x};
                }
            }
        }
    }

    return collided;
}

bool PolygonCollider::cast(
        const Vec &direction,
        const PolygonCollider& otherCollider,
        Vec &contactNormal,
        Vec &penetration
) {
    Vec contactNormalTmp, penetrationTmp;

    bool collided1 = castOneWay_(direction, *this, otherCollider, contactNormal, penetration);
    bool collided2 = castOneWay_(direction * -1, otherCollider, *this, contactNormalTmp, penetrationTmp);

    if (!collided1 && !collided2) {
        return false;
    }

    penetrationTmp *= -1.0;

    if (
            penetrationTmp.lengthSqr() < penetration.lengthSqr()
    ) {
        penetration = penetrationTmp;
        contactNormal = contactNormalTmp;
    }

    contactNormal.normalized();
    if (contactNormal.dot(penetration) > 0) { // flip (is necessary)
        contactNormal = contactNormal * -1;
    }

    return true;
}

PolygonCollider::PolygonCollider(std::vector<Vec> points) {
    points_ = std::move(points);
}

std::vector<Vec> PolygonCollider::points() const {
    return points_;
}

