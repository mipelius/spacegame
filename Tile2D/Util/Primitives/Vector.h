// This file is part of SpaceGame.
// Copyright (C) 2014  Miika Pelkonen
//
// SpaceGame is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// SpaceGame is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SpaceGame.  If not, see <http://www.gnu.org/licenses/>.

#ifndef __Vector_H_
#define __Vector_H_

#include "precompile.h"

class Vector {
public:
    double x;
    double y;
    Vector(double x, double y);
    inline static Vector byAngle(const double &angleDegrees, const double &amount);
    inline Vector operator + (const Vector& otherVector) const;
    inline Vector operator - (const Vector& otherVector) const;
    inline Vector operator -= (const Vector& otherVector);
    inline Vector operator += (const Vector& otherVector);
    inline Vector operator *(double const &amount) const;
    inline double length() const;
    inline double lengthSqr() const;
    inline double angle() const;
};

// inline functions

inline Vector Vector::operator + (const Vector &otherVector) const {
    return {x + otherVector.x, y + otherVector.y};
}

inline Vector Vector::operator - (const Vector &otherVector) const {
    return {x - otherVector.x, y - otherVector.y};
}

inline Vector Vector::operator -= (Vector const &otherVector) {
    x = x - otherVector.x;
    y = y - otherVector.y;
    return {x, y};
}

inline Vector Vector::operator += (const Vector &otherVector) {
    x = x + otherVector.x;
    y = y + otherVector.y;
    return {x, y};
}

inline Vector Vector::operator * (const double &amount) const {
    return {x * amount, y * amount};
}

inline Vector Vector::byAngle(const double &angleDegrees, const double &amount) {
    double angle = angleDegrees / 360 * 2 * M_PI;
    return {
            cos(angle) * amount,
            sin(angle) * amount
    };
}

inline double Vector::length() const {
    return sqrt(x * x + y * y);
}

inline double Vector::lengthSqr() const {
    return x * x + y * y;
}

inline double Vector::angle() const {
    if (y == 0) {
        return 0;
    }

    if (x == 0) {
        if (y > 0) return -90.0;
        if (y < 0) return 90.0;
    }

    return atan2(y, x) * 180 / M_PI;
}

#endif //__Vector_H_
