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



#ifndef SPACEGAME_MATH_H
#define SPACEGAME_MATH_H

#include <cfloat>

#include "Rect.h"

template <typename T>
class Math {
private:
    static const unsigned int randMax = RAND_MAX;

public:
    inline static void clamp(T& value, const T& min, const T& max);
    inline static void clamp(Vec<T>& vec, const Rect& rect);

    inline static T min(T& value1, T& value2);
    inline static T max(T& value1, T& value2);

    inline static bool approx(const T& value1, const T& value2);

    inline static T random(const T& min, const T& max);

    inline static int sign(T value);

    static constexpr float epsilon = FLT_EPSILON;

    inline static T deltaAngle(const T& angleA, const T& angleB);
};

typedef Math<float>     Mathf;
typedef Math<double>    Mathd;
typedef Math<int>       Mathi;

// -- definitions

template <typename T>
inline void Math<T>::clamp(T& value, const T& min, const T& max) {
    if (value < min) {
        value = min;
    }
    else if (value > max) {
        value = max;
    }
}

template <typename T>
inline T Math<T>::min(T& value1, T& value2) {
    return value1 < value2 ? value1 : value2;
}

template <typename T>
inline T Math<T>::max(T& value1, T& value2) {
    return value1 > value2 ? value1 : value2;
}

template<typename T>
bool Math<T>::approx(const T &value1, const T &value2) {
    return abs(value1 - value2) < epsilon;
}

template<typename T>
void Math<T>::clamp(Vec<T> &vec, const Rect &rect) {
    clamp(vec.x, rect.x1, rect.x2);
    clamp(vec.y, rect.y1, rect.y2);
}

template<typename T>
T Math<T>::random(const T &min, const T &max) {
    T interval = max - min;
    return min + interval * ((rand() % randMax) / (float)randMax);
}

template<typename T>
int Math<T>::sign(T value) {
    if (value < 0) {
        return -1;
    }
    if (value == 0) {
        return 0;
    }
    return 1;
}

template<typename T>
T Math<T>::deltaAngle(const T &angleA, const T &angleB) {
    T delta = angleB - angleA;
    if (delta < -360 || delta > 360) {
        delta = (int)delta % 360;
    }
    if (delta < -180) {
        delta = 360 + delta;
    }
    if (delta > 180) {
        delta = -360 + delta;
    }
    return delta;
}

#endif //SPACEGAME_TILE2DMATH_H
