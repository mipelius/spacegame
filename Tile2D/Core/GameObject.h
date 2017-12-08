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
#include "ComponentBase.h"
#include "Tile2DObject.h"

class GameObject : public Tile2DObject {
    friend class Tile2D;
public:
    void addComponent(ComponentBase* component);

protected:
    void onDestroy() final;

private:
    std::list<ComponentBase*> uninitializedComponents_;
    std::list<ComponentBase*> components_;

    void initializeComponents_();
};


#endif //__GameObject_H
