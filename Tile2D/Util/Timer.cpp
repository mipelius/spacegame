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
