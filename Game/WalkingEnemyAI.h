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


#ifndef SPACEGAME_WALKINGENEMYAI_H
#define SPACEGAME_WALKINGENEMYAI_H

#include <SDL2/SDL_quit.h>
#include "Transform.h"
#include "Tile2DBehaviour.h"
#include "Body.h"

class WalkingEnemyAI : public Tile2DBehaviour {
public:
    Transform *getTarget() const;
    void setTarget(Transform *target);
    void setGroundCheckPoints(const std::vector<Vecf> &groundSensors);

protected:
    void awake() override;
    void update() override;
    void lateUpdate() override;

private:
    Body* body_ = nullptr;
    Transform* target_ = nullptr;
    std::vector<Vecf> groundSensors_;

    Uint32 lastJumpTimeStamp_ = 0;
    const Uint32 jumpInterval_ = 6000;
    const Uint32 jumpIntervalRandomness_ = 1000;

    Uint32 lastReactionTimeStamp_ = 0;
    const Uint32 reactionTime_ = 500;
    const Uint32 reactionTimeRandomness_ = 200;

    float xDirection_;

    bool isGrounded_();
};

#endif //SPACEGAME_WALKINGENEMYAI_H
