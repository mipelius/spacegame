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

#ifndef __ReadableProperty_H_
#define __ReadableProperty_H_

template <typename T> class Property;

template <typename T>
class ReadableProperty {

    friend class Property<T>;

public:
    ReadableProperty(void* owner, T (*getter)(void*));
    ReadableProperty(T* actualValue);

    void updateDependentProperties() const;

    T get() const;

private:
    void* owner_;
    T (*getter_)(void*);
    static T defaultGetter(void* owner);

    static const int MAX_DEPENDENT_PROPERTIES_ = 4;

    mutable const Property<T> * dependentProperties_[MAX_DEPENDENT_PROPERTIES_];
    mutable int nextIndexForPropertyToBeBound_ = 0;
};

template <typename T>
ReadableProperty<T>::ReadableProperty(void *owner, T (*getter)(void *)) {
    owner_ = owner;
    getter_ = getter;
}

template <typename T>
ReadableProperty<T>::ReadableProperty(T *actualValue) {
    owner_ = actualValue;
    getter_ = defaultGetter;
}

template <typename T>
T ReadableProperty<T>::get() const {
    return getter_(owner_);
}

template <typename T>
T ReadableProperty<T>::defaultGetter(void* owner) {
    return *(T*)(owner);
}

template <typename T>
void ReadableProperty<T>::updateDependentProperties() const {
    for (int i = 0; i < nextIndexForPropertyToBeBound_; i++) {
        const Property<T>* currentProperty = dependentProperties_[i];
        currentProperty->set(get());
    }
}

#endif //__ReadableProperty_H_

