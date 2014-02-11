// This file is part of SpaceGame.
// Copyright (C) 2014 Miika Pelkonen
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

#ifndef __MusicPlayer_H_
#define __MusicPlayer_H_

class Music;

#include "Player.h"

class MusicPlayer : public Player {

public:
    static MusicPlayer* getInstance();
    void play(Music *music);
    void stop();

private:
    MusicPlayer();

    static void musicFinished();

    static Music* nextMusic_;
    static MusicPlayer* instance_;

    static const int FADING_MS = 500;
};

#endif //__MusicPlayer_H_