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

#include "ReadableProperty.h"
#include "Exception.h"

// definition

template <typename T>
class Property : public ReadableProperty<T> {
    friend class ReadableProperty<T>;

public:
    Property(void* actualData);
    ~Property();

    void set(T value);
    void bind(ReadableProperty<T>* otherProperty);

protected:

    virtual void setActual(T value) = 0;

private:
    ReadableProperty<T>* otherPropertyToBeBoundTo_;
    void setInternal_(T value, std::list<ReadableProperty<T> *>& updatedProperties);
};

// implementation

template <typename T>
Property<T>::Property(void* actualData) : ReadableProperty<T>(actualData) {
    otherPropertyToBeBoundTo_ = nullptr;
}

template <typename T>
Property<T>::~Property() {
    if (otherPropertyToBeBoundTo_) {
        otherPropertyToBeBoundTo_->dependentProperties_.remove(this);
    }
}

template <typename T>
void Property<T>::setInternal_(T value, std::list<ReadableProperty<T> *>& updatedProperties) {
    for (typename std::list<ReadableProperty<T> *>::iterator i = updatedProperties.begin(); i != updatedProperties.end(); i++) {
        if ((*i) == this) return;
    }

    setActual(value);

    updatedProperties.push_back(this);

    std::list<Property<T>*> properties = ReadableProperty<T>::dependentProperties_;

    if (!properties.empty()) {

        typename std::list<Property<T>* >::const_iterator i = properties.begin();

        for (; i != properties.end(); i++) {
            (*i)->setInternal_(this->getActual(), updatedProperties);
        }
    }
}

template <typename T>
void Property<T>::bind(ReadableProperty<T>* otherProperty) {
    if (otherPropertyToBeBoundTo_) {
        throw Exception("Property is already bound.");
    }

    otherPropertyToBeBoundTo_ = otherProperty;
    otherProperty->dependentProperties_.push_back(this);
    setActual(otherPropertyToBeBoundTo_->get());
}

template <typename T>
void Property<T>::set(T value) {
    setActual(value);
    ReadableProperty<T>::updateDependentProperties();
}

#endif //__Property_H_

