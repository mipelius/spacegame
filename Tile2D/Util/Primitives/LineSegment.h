//
// Created by Miika Pelkonen on 15/12/2017.
//

#ifndef __LineSegment_H
#define __LineSegment_H

#include "Vec.h"

class LineSegment {
public:
    Vec point1, point2;

    LineSegment(Vec point1, Vec point2);
    void rotate(float angle);
    bool collision(const LineSegment& staticLine, const Vec& moveDirection, Vec& penetration, Vec& contactNormal) const;
    bool intersection(const LineSegment& other, Vec& result) const;
};


#endif //__LineSegment_H
