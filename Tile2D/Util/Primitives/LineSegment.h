//
// Created by Miika Pelkonen on 15/12/2017.
//

#ifndef __LineSegment_H
#define __LineSegment_H

#include "Vec.h"

class LineSegment {
public:
    Vecf point1, point2;

    LineSegment(Vecf point1, Vecf point2);
    void rotate(float angle);
    bool intersection(const LineSegment& other, Vecf& result) const;
};


#endif //__LineSegment_H
