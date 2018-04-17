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


#ifndef SPACEGAME_POOL_H
#define SPACEGAME_POOL_H

#include <cstdio>
#include <assert.h>

// NOTE: this class has borrowed some ideas from book titled Game Programming Patterns by Robert Nystrom
// and from Boost library

template <typename T>
class Pool {
private:
    union ObjectCell {
        T object;
        ObjectCell* next;

        ObjectCell() : object(T()) { }
    };

    ObjectCell* objectCells_;
    ObjectCell* firstAvailableCell_;

    unsigned int size_;
    unsigned int freeSpace_;

public:
    explicit Pool(unsigned int size);
    ~Pool();

    T* malloc();
    void free(T*& objPtr);

    unsigned int size() const;
    unsigned int freeSpace() const;
};

// -- definitions --

template <typename T>
Pool<T>::Pool(unsigned int size) :
        size_       (size),
        freeSpace_  (size)
{
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
    freeSpace_--;
    return object;
}

template<typename T>
void Pool<T>::free(T*& objPtr) {
    ObjectCell* cell = (ObjectCell*)objPtr;
    cell->next = firstAvailableCell_;
    firstAvailableCell_ = cell;
    freeSpace_++;
    objPtr = nullptr;
}

template<typename T>
unsigned int Pool<T>::size() const {
    return size_;
}

template<typename T>
unsigned int Pool<T>::freeSpace() const {
    return freeSpace_;
}


#endif //SPACEGAME_POOL_H
