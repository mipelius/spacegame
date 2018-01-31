// This file is part of SpaceGame.
// Copyright (C) 2018 Miika Pelkonen
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


#ifndef SPACEGAME_MATH_H
#define SPACEGAME_MATH_H

#include <cfloat>

template <typename T>
class Math {

public:
    inline static void clamp(T& value, const T& min, const T& max);

    inline static T min(T& value1, T& value2);
    inline static T max(T& value1, T& value2);

    inline static bool approx(const T& value1, const T& value2);

    static constexpr float epsilon = FLT_EPSILON;
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

#endif //SPACEGAME_TILE2DMATH_H
