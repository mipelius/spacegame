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


#ifndef __Vector_H_
#define __Vector_H_

#include "precompile.h"
#include <cmath>
#include "ISerializable.h"
#include "json.h"

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

    inline bool operator== (const Vec& otherVector);

    inline T length() const;
    inline T lengthSqr() const;
    inline T angle() const;
    inline T dot(const Vec& otherVector) const;
    inline Vec normalized() const;
};

template <typename T>
std::ostream& operator<<(std::ostream &strm, const Vec<T> &vec) {
    return strm << "(" << vec.x << ", " << vec.y << ")";
}

typedef Vec<double> Vecd;
typedef Vec<int> Veci;


class Vecf : public Vec<float>, public ISerializable
{
public:
    using Vec<float>::Vec;

	Vecf() = default;

    Vecf(Vec<float> otherVector) {
        x = otherVector.x;
        y = otherVector.y;
    }

    inline Vecf& operator = (const Vec<float>& otherVector) {
        x = otherVector.x;
        y = otherVector.y;
        return *this;
    }

    inline void deserialize(const json::Object &jsonObject) override {
        x = jsonObject["x"].ToFloat();
        y = jsonObject["y"].ToFloat();
    }
};


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

template<typename T>
bool Vec<T>::operator==(const Vec &otherVector) {
    return x == otherVector.x && y == otherVector.y;
}

template <typename T>
inline Vec<T> Vec<T>::byAngle(const float &angleDegrees, const float &amount) {
    float angle = angleDegrees / 360 * 2 * M_PI;
    return {
            (T)cos(angle) * amount,
            (T)sin(angle) * amount
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
inline Vec<T> Vec<T>::normalized() const {
    double length = sqrt(x * x + y * y);
    if (length == 0.0) {
        return {1, 0};
    }
    return *this / length;
}

#endif //__Vector_H_
