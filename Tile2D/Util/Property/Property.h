// This file is part of Properties.
// Copyright (C) 2015 Miika Pelkonen
//
// Properties is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Properties is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Properties.  If not, see <http://www.gnu.org/licenses/>.

#ifndef __Property_H_
#define __Property_H_

#include <string>
#include "ReadableProperty.h"

template <typename T>
class Property : public ReadableProperty<T> {

public:
    Property(void *owner, T (*getter)(void *), void (*setter)(void*, const T&));
    Property(T* actualValue);

    void set(const T& value) const;

    void bind(const ReadableProperty<T>& otherProperty) const;

private:
    void init();

    mutable bool isBound_;
    void (*setter_)(void*, const T&);

    static void defaultSetter(void* owner, const T& value);
};

template <typename T>
Property<T>::Property(void *owner, T (*getter)(void *), void (*setter)(void*, const T&)) :
        ReadableProperty<T>(owner, getter)
{
    init();
    setter_ = setter;
}

template <typename T>
Property<T>::Property(T* actualValue) : ReadableProperty<T>(actualValue) {
    init();
    setter_ = defaultSetter;
}
template <typename T>
void Property<T>::init() {
    isBound_ = false;
}


template <typename T>
void Property<T>::bind(const ReadableProperty<T>& otherProperty) const {
    if (&otherProperty == this) {
        throw "Binding failed: property cannot be bound to itself.";
    }

    if (isBound_) {
        throw "Binding failed: property is already bound";
    }

    if (otherProperty.nextIndexForPropertyToBeBound_ >= ReadableProperty<T>::MAX_DEPENDENT_PROPERTIES_) {
        throw "Binding failed: target property has already maximum amount (=" +
                std::to_string(ReadableProperty<T>::MAX_DEPENDENT_PROPERTIES_) +
                        ") of dependent properties.";
    }

    otherProperty.dependentProperties_[otherProperty.nextIndexForPropertyToBeBound_] = this;
    otherProperty.nextIndexForPropertyToBeBound_++;

    this->setter_(ReadableProperty<T>::owner_, otherProperty.get());

    isBound_ = true;
}

template <typename T>
void Property<T>::defaultSetter(void* owner, const T& value) {
    *(T*)(owner) = value;
}

template <typename T>
void Property<T>::set(const T& value) const {
    setter_(ReadableProperty<T>::owner_, value);
    ReadableProperty<T>::updateDependentProperties();
}

#endif //__Property_H_

