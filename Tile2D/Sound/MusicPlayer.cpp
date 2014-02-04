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

#include "precompile.h"
#include "MusicPlayer.h"
#include "Music.h"

Music* MusicPlayer::nextMusic_ = nullptr;

MusicPlayer* MusicPlayer::instance_ = nullptr;

void MusicPlayer::musicFinished() {
    if (!nextMusic_) return;
    Mix_FadeInMusic(nextMusic_->music, -1, FADING_MS);
}

MusicPlayer::MusicPlayer() : Player() {
    Mix_HookMusicFinished(musicFinished);
}

void MusicPlayer::play(Music *music) {
    if (Mix_PlayingMusic()) {
        nextMusic_ = music;
        Mix_FadeOutMusic(FADING_MS);
    }
    else {
        Mix_PlayMusic(music->music, -1);
    }
}

void MusicPlayer::stop() {
    Mix_FadeOutMusic(FADING_MS);
}

MusicPlayer *MusicPlayer::instance() {
    if (!instance_) instance_ = new MusicPlayer();
    return instance_;
}
