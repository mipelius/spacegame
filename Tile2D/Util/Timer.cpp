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



#include "precompile.h"
#include "Timer.h"

Timer::Timer() {
    reset();
}

Timer::~Timer() {

}

Uint32 Timer::getInterval() const {
    return interval_;
}

void Timer::setInterval(Uint32 interval) {
    interval_ = interval;
}

Uint32 Timer::getIntervalRandomness() const {
    return intervalRandomness_;
}

void Timer::setIntervalRandomness(Uint32 intervalRandomness) {
    intervalRandomness_ = intervalRandomness;
}

void Timer::reset() {
    resetTimeStamp_ = SDL_GetTicks();
}

bool Timer::resetIfTimeIntervalPassed() {
    bool needToReset = timeIntervalPassed();
    if (needToReset) {
        reset();
    }
    return needToReset;
}

Timer::Timer(Uint32 interval, Uint32 intervalRandomness) :
        interval_(interval),
        intervalRandomness_(intervalRandomness)
{

}

Uint32 Timer::getTime() {
    return SDL_GetTicks() - resetTimeStamp_;
}

bool Timer::timeIntervalPassed() {
    Uint32 ticks = SDL_GetTicks();

    Uint32 interval = interval_;

    if (intervalRandomness_ > 0) {
        interval += (intervalRandomness_ - rand() % (2 * intervalRandomness_));
    }

    return ticks - resetTimeStamp_ >= interval;
}
