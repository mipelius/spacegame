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

#include "AudioSource.h"
#include "AudioManager.h"
#include "Tile2D.h"
#include "GameObject.h"
#include "Canvas.h"
#include "Camera.h"

AudioManager* AudioManager::instance_ = nullptr;

AudioManager *AudioManager::getInstance() {
    if (instance_ == nullptr) {
        auto gameObject = Tile2D::createGameObject();
        instance_ = gameObject->attachComponent<AudioManager>();
        instance_->gameObject()->setDestroyOnLoad(false);

        for (auto i = 0u; i < MAX_CHANNELS; ++i) {
            auto audioSource = gameObject->attachComponent<AudioSource>();
            instance_->audioContainers_[i] = { audioSource, 0 };
        }
    }

    return instance_;
}

void AudioManager::init() { }

void AudioManager::onDestroy() {
    instance_ = nullptr;
}

Tile2DComponent *AudioManager::clone() {
    throw std::runtime_error("AudioManager: cloning is now allowed!");
}

void AudioManager::play(AudioClip *clip, int volume) {
    auto timeStamp = SDL_GetTicks();

    auto sameClipPlayingCount = 0;
    auto currentAudioContainer = &audioContainers_[currentSourceIndex_];

    for (auto& audioContainer : audioContainers_) {
        if (audioContainer.source->getClip() == clip) {
            if (
                    abs(audioContainer.source->getVolume() - volume) < MIN_VOLUME_DIFF_BETWEEN_PLAYING_SAME_CLIP &&
                    timeStamp - audioContainer.timeStamp < MIN_DELAY_BETWEEN_PLAYING_SAME_CLIP
            ) {
                return;
            }

            if (sameClipPlayingCount == 0) {
                currentAudioContainer = &audioContainer;
            }
            else if (currentAudioContainer->timeStamp > audioContainer.timeStamp) {
                currentAudioContainer = &audioContainer;
            }

            sameClipPlayingCount++;
        }
    }

    if (sameClipPlayingCount <= MAX_COUNT_PLAYING_SAME_CLIP) {
        currentAudioContainer = &audioContainers_[currentSourceIndex_];
        ++currentSourceIndex_;
        currentSourceIndex_ %= audioContainers_.size();
    }

    currentAudioContainer->timeStamp = timeStamp;
    currentAudioContainer->source->setClip(clip);
    currentAudioContainer->source->setVolume(volume);
    currentAudioContainer->source->play();
}

void AudioManager::play(AudioClip *clip, const Vecf& position) {
    auto cameraPosition = Tile2D::canvas().getCamera()->getPosition();
    auto distance = (position - cameraPosition).length();

    if (distance > MAX_DISTANCE) {
        return;
    }

    auto volume = (int)(((MAX_DISTANCE - distance) / MAX_DISTANCE) * 128);

    play(clip, volume);
}

void AudioManager::play(const std::vector<AudioClip *>& audioClips, int volume) {
    if (audioClips.empty()) {
        return;
    }
    auto audioClip = audioClips[rand() % audioClips.size()];
    play(audioClip, volume);
}

void AudioManager::play(const std::vector<AudioClip *>& audioClips, const Vecf &position) {
    if (audioClips.empty()) {
        return;
    }
    auto audioClip = audioClips[rand() % audioClips.size()];
    play(audioClip, position);
}
