//
// Created by Miika Pelkonen on 15/12/2017.
//

#include <iostream>
#include "Manifold.h"
#include "PolygonCollider.h"
#include "LineSegment.h"

bool PolygonCollider::castOneWay_(
        const Vec &direction,
        const PolygonCollider &collider,
        const PolygonCollider &otherCollider,
        Vec &contactNormal,
        Vec &toCollision
) {
    bool collided = false;

    toCollision = {10000.0, 10000.0};

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
                    toCollision = curVec;
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

        Manifold& manifold
) {
    Vec contactNormal, toCollision;
    Vec contactNormalTmp, toCollisionTmp;

    bool collided1 = castOneWay_(direction, *this, otherCollider, contactNormal, toCollision);
    bool collided2 = castOneWay_(direction * -1, otherCollider, *this, contactNormalTmp, toCollisionTmp);

    if (!collided1 && !collided2) {
        return false;
    }

    toCollisionTmp *= -1.0;

    if (
            toCollisionTmp.lengthSqr() < toCollision.lengthSqr()
    ) {
        toCollision = toCollisionTmp;
        contactNormal = contactNormalTmp;
    }

    contactNormal = contactNormal.normalized();
    if (contactNormal.dot(toCollision) > 0) { // flip (is necessary)
        contactNormal = contactNormal * -1;
    }

    double penetration = (direction - toCollision).length();
    manifold = Manifold(contactNormal, penetration);

    return true;
}

PolygonCollider::PolygonCollider(std::vector<Vec> points) : boundingBox_({-1, -1, 1, 1}) {
    points_ = std::move(points);

    Vec farMost = Vec(0, 0);

    for (const auto& vec : points) {
        double length = vec.length();
        double farMostLength = Vec(farMost.x, farMost.y).length();
        if (length > farMostLength) farMost = vec;
    }

    double length = Vec(farMost.x, farMost.y).length();
    boundingBox_ = Rect(-length, -length, length, length);
}

std::vector<Vec> PolygonCollider::points() const {
    return points_;
}

const Rect &PolygonCollider::boundingBox() const {
    return boundingBox_;
}

