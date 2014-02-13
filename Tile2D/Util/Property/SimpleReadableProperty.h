// This file is part of SpaceGame.
// Copyright (C) 2014 Miika Pelkonen
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

#ifndef __SimpleReadableProperty_H_
#define __SimpleReadableProperty_H_

#include "ReadableProperty.h"

// definition

template <typename T>
class SimpleReadableProperty : public ReadableProperty<T> {

public:
    SimpleReadableProperty(T* actualData);

protected:
    T getActual();
};

// implementation

template <typename T>
SimpleReadableProperty<T>::SimpleReadableProperty(T* actualData) : ReadableProperty<T>((void*)actualData) { }

template <typename T>
T SimpleReadableProperty<T>::getActual() {
    return *(T*)this->actualData_;
}

#endif //__SimpleReadableProperty_H_
