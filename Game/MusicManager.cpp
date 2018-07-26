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

#include "Tile2DMath.h"
#include "Tile2D.h"
#include "MusicManager.h"
#include "GameObject.h"
#include "AudioSource.h"

MusicManager* MusicManager::instance_ = nullptr;

MusicManager *MusicManager::getInstance() {
    if (instance_ == nullptr) {
        auto gameObject = Tile2D::createGameObject();
        instance_ = gameObject->attachComponent<MusicManager>();
        instance_->gameObject()->setDestroyOnLoad(false);

        for (auto i = 0u; i < MUSIC_CHANNELS; ++i) {
            auto audioSource = gameObject->attachComponent<AudioSource>();
            instance_->audioSources_[i] = audioSource;
        }

        instance_->currentAudioSource_ = instance_->audioSources_[0];
    }

    return instance_;
}

void MusicManager::play(AudioClip *clip, bool loopEnabled, bool crossFade) {
    // check if clip is already played on some source

    for (auto audioSource : audioSources_) {
        if (audioSource->isPlaying() && audioSource->getClip() == clip) {
            currentAudioSource_ = audioSource;
            currentAudioSource_->setLoopEnabled(loopEnabled);
            return;
        }
    }

    // find free audio source

    for (auto audioSource : audioSources_) {
        if (!audioSource->isPlaying()) {
            currentAudioSource_ = audioSource;
            break;
        }
    }

    currentAudioSource_->setClip(clip);
    currentAudioSource_->setLoopEnabled(loopEnabled);
    if (crossFade) {
        currentAudioSource_->setVolume(0);
    }
    else {
        currentAudioSource_->setVolume(128);
    }

    currentAudioSource_->play();
}

void MusicManager::awake() {

}

void MusicManager::update() {
    for (auto audioSource : audioSources_) {
        auto volume = audioSource->getVolume();

        if (!turnedOn_) {
            volume--;
        }
        else if (audioSource == currentAudioSource_) {
            volume++;
        }
        else {
            volume--;
        }

        Mathi::clamp(volume, 0, 128);
        audioSource->setVolume(volume);

        if (volume <= 0) {
            audioSource->stop();
        }
    }
}

void MusicManager::lateUpdate() {

}

void MusicManager::turnOff() {
    turnedOn_ = false;
}

void MusicManager::turnOn() {
    turnedOn_ = true;
}
