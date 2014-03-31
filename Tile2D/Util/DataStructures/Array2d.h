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

#include <iostream>

template <typename T> class Array2d {
public:
    Array2d<T>(int w, int h) {
        w_ = w;
        h_ = h;
        data_ = new T[w * h];
    }

    ~Array2d<T>() {
        delete[] data_;
    }

    virtual T getValue(int x, int y) const {
        return data_[x + y * w_];
    }

    virtual void setValue(int x, int y, T value) {
        data_[x + y * w_] = value;
    }

    bool isInsideBounds(int x, int y) {
        if (x < 0 || x >= w_) {
            return false;
        }
        if (y < 0 || y >= h_) {
            return false;
        }

        return true;
    }

    int getW() {
        return w_;
    }

    int getH() {
        return h_;
    }

protected:

    int w_;
    int h_;
    T* data_;
};


#endif //__Array2d_H_
