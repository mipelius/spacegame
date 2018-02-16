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


#ifndef SPACEGAME_FLYINGENEMYAI_H
#define SPACEGAME_FLYINGENEMYAI_H

#include "EnemyAIBase.h"
#include "Timer.h"
#include "PolygonCollider.h"

class FlyingEnemyAI : public EnemyAIBase {
public:
    float getMaxPathFindingDistance() const;
    void setMaxPathFindingDistance(float maxPathFindingDistance);
    const Timer &getPathUpdateTimer() const;
    void setPathUpdateTimer(const Timer &pathUpdateTimer);

protected:
    void awake() override;
    void update() override;
    void lateUpdate() override;

    std::list<Vecf> pathToTarget_;
    Vecf lastPoint_;
    Vecf nextPoint_;

private:
    PolygonCollider* collider_;
    float maxPathFindingDistance_ = 1200.0f;
    void updateNextPoint_();
    Timer pathUpdateTimer_;
};

#endif //SPACEGAME_FLYINGENEMYAI_H
