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

#include <cmath>

class Vec {
public:
    float x;
    float y;
    Vec(float x, float y);
    Vec();

    inline void rotate(float degrees);
    inline Vec rotated(float degrees) const;
    inline static Vec byAngle(const float &angleDegrees, const float &amount);

    inline Vec operator + (const Vec& otherVector) const;
    inline Vec operator - (const Vec& otherVector) const;
    inline Vec operator * (float const &amount) const;
    inline Vec operator / (float const &amount) const;

    inline Vec operator -= (const Vec& otherVector);
    inline Vec operator += (const Vec& otherVector);
    inline Vec operator *= (float const &amount);
    inline Vec operator /= (float const &amount);

    inline float length() const;
    inline float lengthSqr() const;
    inline float angle() const;
    inline float dot(const Vec& otherVector) const;
    inline Vec normalized();
};

// inline functions

inline void Vec::rotate(float degrees) {
    *this = this->rotated(degrees);
}

inline Vec Vec::rotated(float degrees) const {
    degrees = (degrees / 180.0) * M_PI;
    return {Vec(cos(degrees), -sin(degrees)).dot(*this), Vec(sin(degrees), cos(degrees)).dot(*this)};
}

inline Vec Vec::operator + (const Vec &otherVector) const {
    return {x + otherVector.x, y + otherVector.y};
}

inline Vec Vec::operator - (const Vec &otherVector) const {
    return {x - otherVector.x, y - otherVector.y};
}

inline Vec Vec::operator * (const float &amount) const {
    return {x * amount, y * amount};
}

inline Vec Vec::operator /(float const &amount) const {
    return {x / amount, y / amount};
}

inline Vec Vec::operator -= (Vec const &otherVector) {
    x = x - otherVector.x;
    y = y - otherVector.y;
    return {x, y};
}

inline Vec Vec::operator += (const Vec &otherVector) {
    x = x + otherVector.x;
    y = y + otherVector.y;
    return {x, y};
}

inline Vec Vec::operator *=(float const &amount) {
    x *= amount;
    y *= amount;
    return {x, y};
}

inline Vec Vec::operator /=(float const &amount) {
    x /= amount;
    y /= amount;
    return {x, y};
}

inline Vec Vec::byAngle(const float &angleDegrees, const float &amount) {
    float angle = angleDegrees / 360 * 2 * M_PI;
    return {
            cos(angle) * amount,
            sin(angle) * amount
    };
}

inline float Vec::length() const {
    return sqrt(x * x + y * y);
}

inline float Vec::lengthSqr() const {
    return x * x + y * y;
}

inline float Vec::angle() const {
    if (y == 0) {
        return 0;
    }

    if (x == 0) {
        if (y > 0) return -90.0;
        if (y < 0) return 90.0;
    }

    return atan2(y, x) * 180 / M_PI;
}

inline float Vec::dot(const Vec& otherVector) const {
    return this->x * otherVector.x + this->y * otherVector.y;
}

inline Vec Vec::normalized() {
    float length = this->length();
    if (length == 0.0) {
        return {1, 0};
    }
    return *this / length;
}

#endif //__Vector_H_
