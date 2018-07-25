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

#include "Tile2D.h"
#include "Mixer.h"
#include "AudioSource.h"
#include "Resources.h"

void AudioSource::play() {
    if (clip_ == nullptr) {
        return;
    }
    auto loops = loopEnabled_ ? -1 : 0;

    Mix_PlayChannel(SDL_mixer_channel_, clip_->chunk_, loops);
}

void AudioSource::stop() {
    Mix_HaltChannel(SDL_mixer_channel_);
}

void AudioSource::pause() {
    Mix_Pause(SDL_mixer_channel_);
}

bool AudioSource::isPlaying() {
    return Mix_Playing(SDL_mixer_channel_) == 1;
}

void AudioSource::init() {
    Mix_Volume(SDL_mixer_channel_, volume_);

    if (playOnAwake_) {
        play();
    }
}

void AudioSource::onDestroy() {
    stop();
    Tile2D::mixer().freeChannel_(SDL_mixer_channel_);
}

Tile2DComponent *AudioSource::clone() {
    return new AudioSource(*this);
}

AudioClip *AudioSource::getClip() const {
    return clip_;
}

void AudioSource::setClip(AudioClip *clip) {
    clip_ = clip;
}

bool AudioSource::isLoopEnabled() const {
    return loopEnabled_;
}

void AudioSource::setLoopEnabled(bool loopEnabled) {
    loopEnabled_ = loopEnabled;
}

void AudioSource::deserialize(const json::Object &jsonObject) {
    if (jsonObject.HasKey("playOnAwake")) {
        playOnAwake_ = jsonObject["playOnAwake"].ToBool();
    }
    if (jsonObject.HasKey("loopEnabled")) {
        loopEnabled_ = jsonObject["loopEnabled"].ToBool();
    }
    if (jsonObject.HasKey("audioClip")) {
        auto audioClipName = jsonObject["audioClip"].ToString();
        clip_ = Tile2D::resources().audioClips[audioClipName];
    }
    if (jsonObject.HasKey("volume")) {
        volume_ = jsonObject["volume"].ToInt();
    }
}

bool AudioSource::isPlayOnAwake() const {
    return playOnAwake_;
}

void AudioSource::setPlayOnAwake(bool playOnAwake) {
    playOnAwake_ = playOnAwake;
}

int AudioSource::getVolume() const {
    return volume_;
}

void AudioSource::setVolume(int volume) {
    volume_ = volume;
    if (SDL_mixer_channel_ != -1) {
        Mix_Volume(SDL_mixer_channel_, volume_);
    }
}

AudioSource::AudioSource() {
    SDL_mixer_channel_ = Tile2D::mixer().reserveChannel_();
}
