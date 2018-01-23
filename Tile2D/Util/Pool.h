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

#ifndef SPACEGAME_POOL_H
#define SPACEGAME_POOL_H

#import <cstdio>
#import <assert.h>

// NOTE: this class has borrowed some ideas from book titled Game Programming Patterns by Robert Nystrom
// and from Boost library

template <typename T>
class Pool {
    int i=0;

private:
    union ObjectCell {
        T object;
        ObjectCell* next;
    };

    ObjectCell* objectCells_;
    ObjectCell* firstAvailableCell_;

public:
    const unsigned int size;

    explicit Pool(unsigned int size);
    ~Pool();

    T* malloc();
    void free(T*& objPtr);
};

// -- definitions --

template <typename T>
Pool<T>::Pool(unsigned int size) : size(size) {
    objectCells_ = new ObjectCell[size];

    firstAvailableCell_ = &objectCells_[0];

    for (auto i=0u; i<size-1; ++i) {
        objectCells_[i].next = &objectCells_[i+1];
    }

    objectCells_[size - 1].next = nullptr;
}

template <typename T>
Pool<T>::~Pool() {
    delete[] objectCells_;
}

template <typename T>
T* Pool<T>::malloc() {
    assert(firstAvailableCell_ != nullptr);
    T* object = &firstAvailableCell_->object;
    firstAvailableCell_ = firstAvailableCell_->next;
    return object;
}

template<typename T>
void Pool<T>::free(T*& objPtr) {
    ObjectCell* cell = (ObjectCell*)objPtr;
    cell->next = firstAvailableCell_;
    firstAvailableCell_ = cell;

    objPtr = nullptr;
}


#endif //SPACEGAME_POOL_H
