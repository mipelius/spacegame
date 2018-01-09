//
// Created by Miika Pelkonen on 15/12/2017.
//

#include <iostream>
#include <cfloat>
#include "PolygonCollider.h"
#include "LineSegment.h"

class Projection {

public:
    Projection(const Vec& axis, const PolygonCollider& collider) {
        min = axis.dot(collider.points[0].rotated(collider.rot) + collider.pos);
        max = min;
        for (auto i = 1u; i < collider.points.size(); ++i) {
            double p = axis.dot(collider.points[i].rotated(collider.rot) + collider.pos);
            if (p < min) {
                min = p;
            } else if (p > max) {
                max = p;
            }
        }
    }

    bool overlap(const Projection& other, double& penetration, Vec& currentContactNormal) {
        penetration = DBL_MAX;
        bool overlap = (min >= other.min || max >= other.min) && (min <= other.max || max <= other.max);
        if (!overlap) { return false; }

        if (min <= other.min && max >= other.min) {
            penetration = max - other.min;
            currentContactNormal *= -1;
            return true;
        } else if (min <= other.max && max >= other.max) {
            if (min <= other.min) {

            }
            penetration = other.max - min;
            return true;
        }
        return overlap;
    }

    double min, max;
};

bool PolygonCollider::castOneWay_(
        const Vec &direction,
        const PolygonCollider &collider,
        const PolygonCollider &otherCollider,
        Vec &contactNormal,
        Vec &toCollision
) {
    bool collided = false;

    toCollision = {0.0, 0.0};

    const PolygonCollider& c1 = collider;
    const PolygonCollider& c2 = otherCollider;

    const Vec& c1pos = collider.pos;
    const double& c1rot = collider.rot;
    const Vec& c2pos = otherCollider.pos;
    const double& c2rot = otherCollider.rot;

    Vec intersectionPoint;
    double maxDistanceSqr = 0.0f;
    double curDistanceSqr;
    Vec curVec;

    for (const auto& pointOrig : c1.points) {
        Vec castingPoint = pointOrig.rotated(c1rot) + c1pos;
        LineSegment castingLine(castingPoint, castingPoint + direction);

        for (auto i = 0u; i < otherCollider.points.size(); i++) {
            Vec startPoint = c2.points[i].rotated(c2rot) + c2pos;
            Vec endPoint = c2.points[(i+1) % c2.points.size()].rotated(c2rot) + c2pos;
            LineSegment polygonLine(startPoint, endPoint);

            if (castingLine.intersection(polygonLine, intersectionPoint)) {
                collided = true;
                curVec = (intersectionPoint - castingPoint);
                curDistanceSqr = curVec.lengthSqr();
                if (maxDistanceSqr < curDistanceSqr) {
                    maxDistanceSqr = curDistanceSqr;
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
        Vec& contactNormal,
        Vec& toCollision
) {
    Vec contactNormalTmp, toCollisionTmp;

    bool collided1 = castOneWay_(direction, *this, otherCollider, contactNormal, toCollision);
    bool collided2 = castOneWay_(direction * -1, otherCollider, *this, contactNormalTmp, toCollisionTmp);

    if (!collided1 && !collided2) {
        return false;
    }

    toCollisionTmp *= -1.0;

    if (
            toCollisionTmp.lengthSqr() > toCollision.lengthSqr()
    ) {
        toCollision = toCollisionTmp;
        contactNormal = contactNormalTmp;
    }

    contactNormal = contactNormal.normalized();
    if (contactNormal.dot(toCollision) < 0) { // flip (if necessary)
        contactNormal = contactNormal * -1;
    }

    return true;
}

PolygonCollider::PolygonCollider(std::vector<Vec> points) : boundingBox_({-1, -1, 1, 1}) {
    this->points = std::move(points);

    Vec farMost = Vec(0, 0);

    for (const auto& vec : this->points) {
        double length = vec.length();
        double farMostLength = Vec(farMost.x, farMost.y).length();
        if (length > farMostLength) farMost = vec;
    }

    double length = Vec(farMost.x, farMost.y).length();
    boundingBox_ = Rect(-length, -length, length, length);
}

const Rect &PolygonCollider::boundingBox() const {
    return boundingBox_;
}


bool PolygonCollider::overlap(const PolygonCollider &otherCollider, Vec& contactNormal, double& penetration) const {
    penetration = DBL_MAX;
    double currentPenetration;
    Vec currentContactNormal;

    std::vector<Vec> axes1 = getAxes();
    std::vector<Vec> axes2 = otherCollider.getAxes();

    for (const auto& axis : axes1) {
        Projection p1(axis, *this);
        Projection p2(axis, otherCollider);
        currentContactNormal = axis;
        if (!p1.overlap(p2, currentPenetration, currentContactNormal)) {
            return false;
        } else if (currentPenetration < penetration) {
            penetration = currentPenetration;
            contactNormal = currentContactNormal;
        }
    }

    for (const auto& axis : axes2) {
        Projection p1(axis, *this);
        Projection p2(axis, otherCollider);
        currentContactNormal = axis;
        if (!p1.overlap(p2, currentPenetration, currentContactNormal)) {
            return false;
        } else if (currentPenetration < penetration){
            penetration = currentPenetration;
            contactNormal = currentContactNormal;
        }
    }

    return true;
}

std::vector<Vec> PolygonCollider::getAxes() const {
    std::vector<Vec> axes;

    for (auto i = 0u; i < points.size(); i++) {
        const Vec& p1 = points[i];
        const Vec& p2 = points[(i+1) % points.size()];
        Vec direction = (p2 - p1).rotated(rot);
        Vec vec = Vec(direction.y, -direction.x).normalized();
        axes.push_back(vec);
    }

    return axes;
}
