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

#ifndef __Property_H_
#define __Property_H_

#include <list>
#include "Exception.h"

template<typename T> class Property
{
    friend class Property;

public:
    Property(T initialValue);
    ~Property();

    T get();
    void set(T);

    void bind(Property<T>* otherProperty);
protected:
    T value_;

    virtual T getActual();
    virtual void setActual(T value);

private:
    std::list<Property<T>*> boundProperties_;
    Property<T>*otherPropertyToBeBoundTo_;

    void setInternal_(T value, Property<T>* setter);
};

template <typename T> Property<T>::Property(T initialValue) : value_(initialValue) {
    otherPropertyToBeBoundTo_ = nullptr;
}

template <typename T> Property<T>::~Property() {
    if (otherPropertyToBeBoundTo_) {
        otherPropertyToBeBoundTo_->boundProperties_.remove(this);
    }
}

template <typename T> T Property<T>::get() {
    return this->getActual();
}


template <typename T> void Property<T>::setInternal_(T value, Property<T>* setter) {
    if (setter == this) {
        return;
    }

    setActual(value);

    if (!boundProperties_.empty()) {
        for (typename std::list<Property<T>*>::const_iterator i = boundProperties_.begin(); i != boundProperties_.end(); i++) {
            (*i)->setInternal_(value, setter);
        }
    }
}

template <typename T> void Property<T>::set(T value) {
    setActual(value);

    if (!boundProperties_.empty()) {
        for (typename std::list<Property<T>*>::const_iterator i = boundProperties_.begin(); i != boundProperties_.end(); i++) {
            (*i)->setInternal_(value_, this);
        }
    }
}

template <typename T> void Property<T>::bind(Property<T>* otherProperty) {
    if (otherPropertyToBeBoundTo_) {
        throw Exception("Property is already bound.");
    }

    otherPropertyToBeBoundTo_ = otherProperty;
    otherProperty->boundProperties_.push_back(this);
    setActual(otherPropertyToBeBoundTo_->get());
}

template <typename T> void Property<T>::setActual(T value) {
    value_ = value;
}

template <typename T> T Property<T>::getActual() {
    return value_;
}

#endif //__Property_H_
