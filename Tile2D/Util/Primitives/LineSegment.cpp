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

#include <stdlib.h>
#include "LineSegment.h"

LineSegment::LineSegment(Vecf point1, Vecf point2) {
    this->point1 = point1;
    this->point2 = point2;
}

void LineSegment::rotate(float angle) {
    Vecf middlePoint = point1 + (point2 - point1) / 2.0;
    point1 = point1 - middlePoint;
    point1.rotate(angle);
    point1 = point1 + middlePoint;

    point2 = point2 - middlePoint;
    point1.rotate(angle);
    point2 = point2 + middlePoint;
}

bool LineSegment::intersection(const LineSegment& other, Vecf& result) const {
    auto& ax1 = point1.x;       auto& ay1 = point1.y;       auto& ax2 = point2.x;       auto& ay2 = point2.y;
    auto& bx1 = other.point1.x; auto& by1 = other.point1.y; auto& bx2 = other.point2.x; auto& by2 = other.point2.y;

    auto& x = result.x; auto& y = result.y;

    // --- vertical cases ---

    const float epsilon = 0.0000001;

    bool a_is_vert = abs(ax2 - ax1) < epsilon;
    bool b_is_vert = abs(bx2 - bx1) < epsilon;

    if (a_is_vert && b_is_vert) {
        return false;
    } else if (a_is_vert) {
        x = ax1;
        float slope_b = (by2 - by1) / (bx2 - bx1);
        y = slope_b * (x - bx1) + by1;
    } else if (b_is_vert) {
        x = bx1;
        float slope_a = (ay2 - ay1) / (ax2 - ax1);
        y = slope_a * (x - ax1) + ay1;
    } else {
        // --- non-vertical cases ---

        float slope_a = (ay2 - ay1) / (ax2 - ax1);
        float slope_b = (by2 - by1) / (bx2 - bx1);

        float tmp_a = ay1 - slope_a * ax1;
        float tmp_b = by1 - slope_b * bx1;

        x = (tmp_b - tmp_a) / (slope_a - slope_b);
        y = tmp_a + slope_a * x;
    }

    bool intersection_a =
            ((ax1 <= x && x <= ax2) || (ax1 >= x && x >= ax2)) &&
            ((ay1 <= y && y <= ay2) || (ay1 >= y && y >= ay2));

    bool intersection_b =
            ((bx1 <= x && x <= bx2) || (bx1 >= x && x >= bx2)) &&
            ((by1 <= y && y <= by2) || (by1 >= y && y >= by2));

    return intersection_a && intersection_b;
}