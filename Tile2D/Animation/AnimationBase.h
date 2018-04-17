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


#ifndef __AnimationBase_H_
#define __AnimationBase_H_

#include "IAnimation.h"
#include "Event.h"

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
