// This file is part of SpaceGame.
// Copyright (C) 2014  Miika Pelkonen
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

#ifndef __AnimationBase_H_
#define __AnimationBase_H_

#include "IAnimation.h"
#include "Event.h"
#include "EventArgs.h"

class AnimationBase : public IAnimation {

public:
    AnimationBase(unsigned int framesPerSecond, unsigned int frameAmount, bool enableLoop);

    void play();
    virtual void stop();
    void pause();

protected:
    void update(float seconds) override;
    virtual void updateActual() = 0;

    float timeElapsedAfterPreviousFrame_;
    unsigned int currentFrame_;
    unsigned int fps_;
    unsigned int frameAmount_;

    bool isPlaying_;
    bool isStopped_;

    bool loopEnabled_;
};


#endif //__AnimationBase_H_
