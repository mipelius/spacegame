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


#ifndef SPACEGAME_TIMER_H
#define SPACEGAME_TIMER_H


#include <SDL2/SDL_quit.h>

class Timer {
public:
    Timer();
    Timer(Uint32 interval, Uint32 intervalRandomness);

    ~Timer();

    void reset();
    bool resetIfTimeIntervalPassed();

    Uint32 getInterval() const;
    void setInterval(Uint32 interval);
    Uint32 getIntervalRandomness() const;
    void setIntervalRandomness(Uint32 intervalRandomness);

    bool timeIntervalPassed();

    Uint32 getTime();

private:
    Uint32 resetTimeStamp_ = 0;

    Uint32 interval_ = 0;
    Uint32 intervalRandomness_ = 0;

};


#endif //SPACEGAME_TIMER_H
