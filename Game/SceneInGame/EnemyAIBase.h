// This file is part of SpaceGame.
// Copyright (C) 2018 Miika Pelkonen
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


#ifndef SPACEGAME_ENEMYAIBASE_H
#define SPACEGAME_ENEMYAIBASE_H

#include "Tile2DBehaviour.h"
#include "Body.h"

class EnemyAIBase : public Tile2DBehaviour {

public:
    void setTarget(Transform *target);
    Transform *getTarget() const;

protected:
    Body* body_;
    Transform* target_ = nullptr;

    void shootTarget_();
    void awake() override;
};

#endif //SPACEGAME_ENEMYAIBASE_H
