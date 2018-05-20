// MIT License
//
// This file is part of SpaceGame.
// Copyright (c) 2014-2018 Miika Pelkonen
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.



#ifndef SPACEGAME_FLYINGENEMYAI_H
#define SPACEGAME_FLYINGENEMYAI_H

#include "EnemyAIBase.h"
#include "CountDownTimer.h"
#include "PolygonCollider.h"

class FlyingEnemyAI : public EnemyAIBase {
public:
    float getMaxPathFindingDistance() const;
    void setMaxPathFindingDistance(float maxPathFindingDistance);
    unsigned int getMaxNodesPathFinderExplores() const;
    void setMaxNodesPathFinderExplores(unsigned int maxNodesPathFinderExplores);
    float getMinPathFindingDistance() const;
    void setMinPathFindingDistance(float minPathFindingDistance);
    float getSpeed() const;
    void setSpeed(float speed);
    float getAngularSpeed() const;
    void setAngularSpeed(float angularSpeed);
    bool rotates() const;
    void setRotates(bool rotates);
    bool flips() const;
    void setFlips(bool flips);

    void setPathFindingInterval(Uint32 millisec);

protected:
    void awake() override;
    void update() override;
    void lateUpdate() override;

    std::list<Vecf> pathToTarget_;
    Vecf nextPoint_;

    float rotateTowards_(Vecf direction);

private:
    PolygonCollider* collider_;
    float minPathFindingDistance_ = 350.0f;
    float maxPathFindingDistance_ = 2000.0f;
    float speed = 20;
    float angularSpeed_ = 100.0f;
    bool rotates_ = true;
    bool flips_ = false;

    unsigned int maxNodesPathFinderExplores_ = 1000;
    void updateNextPoint_();
    CountDownTimer pathUpdateTimer_;
};

#endif //SPACEGAME_FLYINGENEMYAI_H
