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

template <typename T>
class Math {

public:
    inline static void clamp(T& value, T min, T max);
};

typedef Math<float>     Mathf;
typedef Math<double>    Mathd;
typedef Math<int>       Mathi;

// -- definitions

template <typename T>
inline void Math<T>::clamp(T& value, T min, T max) {
    if (value < min) {
        value = min;
    }
    else if (value > max) {
        value = max;
    }
}

#endif //SPACEGAME_TILE2DMATH_H
