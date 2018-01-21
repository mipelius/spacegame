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

template <typename T>
class Vec {

public:
    T x;
    T y;
    Vec(T x, T y);
    Vec();

    inline void rotate(float degrees);
    inline Vec rotated(float degrees) const;
    inline static Vec byAngle(const float &angleDegrees, const float &amount);

    inline Vec operator + (const Vec& otherVector) const;
    inline Vec operator - (const Vec& otherVector) const;
    inline Vec operator * (T const &amount) const;
    inline Vec operator / (T const &amount) const;

    inline Vec operator -= (const Vec& otherVector);
    inline Vec operator += (const Vec& otherVector);
    inline Vec operator *= (T const &amount);
    inline Vec operator /= (T const &amount);

    inline T length() const;
    inline T lengthSqr() const;
    inline T angle() const;
    inline T dot(const Vec& otherVector) const;
    inline Vec normalized();
};

typedef Vec<double> Vecd;
typedef Vec<float> Vecf;
typedef Vec<int> Veci;

// -------- definitions --------

template <typename T>
Vec<T>::Vec(T x, T y) {
    this->x = x;
    this->y = y;
}

template <typename T>
Vec<T>::Vec() : Vec<T>(0, 0) {}

template <typename T>
inline void Vec<T>::rotate(float degrees) {
    *this = this->rotated(degrees);
}

template <typename T>
inline Vec<T> Vec<T>::rotated(float degrees) const {
    degrees = (degrees / 180.0f) * M_PI;
    return {Vec(cos(degrees), -sin(degrees)).dot(*this), Vec(sin(degrees), cos(degrees)).dot(*this)};
}

template <typename T>
inline Vec<T> Vec<T>::operator + (const Vec<T> &otherVector) const {
    return {x + otherVector.x, y + otherVector.y};
}

template <typename T>
inline Vec<T> Vec<T>::operator - (const Vec<T> &otherVector) const {
    return {x - otherVector.x, y - otherVector.y};
}

template <typename T>
inline Vec<T> Vec<T>::operator * (const T &amount) const {
    return {x * amount, y * amount};
}

template <typename T>
inline Vec<T> Vec<T>::operator /(const T &amount) const {
    return {x / amount, y / amount};
}

template <typename T>
inline Vec<T> Vec<T>::operator -= (Vec<T> const &otherVector) {
    x = x - otherVector.x;
    y = y - otherVector.y;
    return {x, y};
}

template <typename T>
inline Vec<T> Vec<T>::operator += (const Vec<T> &otherVector) {
    x = x + otherVector.x;
    y = y + otherVector.y;
    return {x, y};
}

template <typename T>
inline Vec<T> Vec<T>::operator *=(T const &amount) {
    x *= amount;
    y *= amount;
    return {x, y};
}

template <typename T>
inline Vec<T> Vec<T>::operator /=(T const &amount) {
    x /= amount;
    y /= amount;
    return {x, y};
}

template <typename T>
inline Vec<T> Vec<T>::byAngle(const float &angleDegrees, const float &amount) {
    float angle = angleDegrees / 360 * 2 * M_PI;
    return {
            cos(angle) * amount,
            sin(angle) * amount
    };
}

template <typename T>
inline T Vec<T>::length() const {
    return (T)sqrt(x * x + y * y);
}

template <typename T>
inline T Vec<T>::lengthSqr() const {
    return x * x + y * y;
}

template <typename T>
inline T Vec<T>::angle() const {
    if (y == 0) {
        return 0;
    }

    if (x == 0) {
        if (y > 0) return -90.0;
        if (y < 0) return 90.0;
    }

    return (T)(atan2(y, x) * 180 / M_PI);
}

template <typename T>
inline T Vec<T>::dot(const Vec& otherVector) const {
    return this->x * otherVector.x + this->y * otherVector.y;
}

template <typename T>
inline Vec<T> Vec<T>::normalized() {
    double length = sqrt(x * x + y * y);
    if (length == 0.0) {
        return {1, 0};
    }
    return *this / length;
}

#endif //__Vector_H_
