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

#ifndef __ReadableProperty_H_
#define __ReadableProperty_H_

template<typename T>
class Property;

#include <list>

// definition

template<typename T>
class ReadableProperty {
    friend class Property<T>;

public:
    ReadableProperty(void* actualData);
    virtual ~ReadableProperty();

    T get();

    void updateDependentProperties();

protected:
    void* actualData_;
    virtual T getActual() = 0;

private:
    std::list<Property<T>*> dependentProperties_;
    std::list<ReadableProperty<T>*> updatedProperties_;
};

// implementation

template <typename T>
ReadableProperty<T>::ReadableProperty(void* actualData) : actualData_(actualData) { }

template <typename T>
ReadableProperty<T>::~ReadableProperty() { }

template <typename T>
T ReadableProperty<T>::get() {
    return getActual();
}

template <typename T>
void ReadableProperty<T>::updateDependentProperties() {
    if (!dependentProperties_.empty()) {
        for (typename std::list<Property<T>* >::iterator i = dependentProperties_.begin(); i != dependentProperties_.end(); i++) {

            updatedProperties_.push_back(this);

            (*i)->Property<T>::setInternal_(getActual(), updatedProperties_);

            updatedProperties_.clear();
        }
    }
}

#endif //__ReadableProperty_H_
