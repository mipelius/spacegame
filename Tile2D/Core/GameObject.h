// This file is part of SpaceGame.
// Copyright (C) 2017 Miika Pelkonen
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

#ifndef __GameObject_H
#define __GameObject_H

#include <list>
#include "Tile2DComponent.h"
#include "Tile2DObject.h"

class GameObject : public Tile2DObject {
    friend class Tile2D;
public:
    void addComponent(Tile2DComponent* component);

    template <class T>
    T* getComponent<T>();

protected:
    void onDestroy() final;
    void onCreate() final;

private:
    std::list<Tile2DComponent*> uninitializedComponents_;
    std::list<Tile2DComponent*> components_;

    void initializeComponents_();
};

// template function

template <class T>
T* GameObject::getComponent<T>() {
    for (auto& component : uninitializedComponents_) {
        auto currentComponent = dynamic_cast<T>(component);
        if (currentComponent != nullptr) {
            return currentComponent;
        }
    }

    for (auto& component : components_) {
        auto currentComponent = dynamic_cast<T>(component);
        if (currentComponent != nullptr) {
            return currentComponent;
        }
    }

    return nullptr;
}

#endif //__GameObject_H
