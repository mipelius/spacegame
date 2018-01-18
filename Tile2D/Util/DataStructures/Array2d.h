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

#ifndef __Array2d_H_
#define __Array2d_H_

template <typename T> class Array2d {
public:
    Array2d<T>(int w, int h) : w(w), h(h) {
        data = new T[w * h];
    }

    ~Array2d<T>() {
        delete[] data;
    }

    inline virtual T getValue(int x, int y) const {
        return data[x + y * w];
    }

    inline virtual void setValue(int x, int y, const T& value) {
        data[x + y * w] = value;
    }

    inline bool isInsideBounds(int x, int y) {
        return !(x < 0 || x >= w) && !(y < 0 || y >= h);
    }

    inline void fill(T value) {
        for (auto x=0; x<w; ++x)
            for (auto y=0; y<h; ++y)
                setValue(x, y, value);
    }

    const int w;
    const int h;
    T* data;
};


#endif //__Array2d_H_
