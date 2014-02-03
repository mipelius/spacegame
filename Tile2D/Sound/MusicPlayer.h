// This file is part of JustFoolingAround.
// Copyright (C) 2014 Miika Pelkonen
//
// JustFoolingAround is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// JustFoolingAround is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with JustFoolingAround.  If not, see <http://www.gnu.org/licenses/>.

#ifndef __MusicPlayer_H_
#define __MusicPlayer_H_

class Music;

#include "Player.h"

class MusicPlayer : public Player {

public:
    MusicPlayer();
    void play(Music *music);
};

#endif //__MusicPlayer_H_
