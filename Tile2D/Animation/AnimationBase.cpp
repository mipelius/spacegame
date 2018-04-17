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


#include "AnimationBase.h"

AnimationBase::AnimationBase(unsigned int framesPerSecond, unsigned int frameAmount, bool enableLoop)
{
    fps_ = framesPerSecond;
    frameAmount_ = frameAmount;
    loopEnabled_ = enableLoop;

    AnimationBase::stop();
}

void AnimationBase::update(float seconds) {
    if (isStopped_ || !isPlaying_) return;

    timeElapsedAfterPreviousFrame_ += seconds;

    if (timeElapsedAfterPreviousFrame_ >= 1.0 / fps_) {
        timeElapsedAfterPreviousFrame_ = 0;
        currentFrame_++;

        if (currentFrame_ >= frameAmount_) {
            currentFrame_ = 0;

            if (!loopEnabled_) {
                stop();
            }
        }

        updateActual();
    }
}

void AnimationBase::stop() {
    timeElapsedAfterPreviousFrame_ = 0;
    currentFrame_ = 0;
    isStopped_ = true;
    isPlaying_ = false;
}

void AnimationBase::play() {
    isStopped_ = false;
    isPlaying_ = true;
}

void AnimationBase::pause() {
    isPlaying_ = false;
}
