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

#ifndef __math_utils_H
#define __math_utils_H

#include <cmath>

class MathUtils {

private:
    static const int SIZE = 2048;
    unsigned int distances[SIZE][SIZE];

private:
    MathUtils() {
        for (auto x = 0u; x < SIZE; ++x) {
            for (auto y = 0u; y < SIZE; ++y) {
                distances[x][y] = (unsigned int)sqrt(x * x + y * y);
            }
        }
    }

public:
    static unsigned int getLength(int x, int y) {
        static MathUtils instance = MathUtils();

        if (x < 0) {
            x = -x;
        }
        if (y < 0) {
            y = -y;
        }

        return instance.distances[x][y];
    }
};

#endif //__math_utils_H
