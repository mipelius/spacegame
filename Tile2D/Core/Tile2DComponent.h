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

#ifndef __IComponent_H
#define __IComponent_H

#include "Tile2DObject.h"
#include "GameObject.h"

class Tile2DComponent : public Tile2DObject {
    friend class GameObject;

public:
    GameObject* gameObject() { return gameObject_; }

protected:
    virtual void init() = 0;

private:
    GameObject* gameObject_ = nullptr;

};

#endif //__IComponent_H
