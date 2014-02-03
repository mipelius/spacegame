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

#include "precompile.h"
#include "MusicPlayer.h"
#include "Music.h"

MusicPlayer::MusicPlayer() : Player() { }

void MusicPlayer::play(Music *music) {
    Mix_PlayMusic(music->music, 0);
}