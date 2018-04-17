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
#include "SamplePlayer.h"
#include "AudioClip.h"
#include "Ears.h"

SamplePlayer::SamplePlayer() : Player() {
    ears_ = nullptr;
}


void SamplePlayer::play(AudioClip *sample, const Vecf &position, int channel) {
    if (ears_ == nullptr) {
        Mix_SetPanning(channel, 255, 255);
    }
    else {
        float distance = (position - ears_->getPosition()).length();

        if (distance < ears_->getMaxDistance()) {
            float amount = 1.0 - (distance / ears_->getMaxDistance());

            if (amount < 0) {
                amount = 0;
            }

            float deltaX = position.x - ears_->getPosition().x;

            Uint8 left;
            Uint8 right;

            if (deltaX < 0.0) {
                left = (Uint8)(255 * amount);
                right = (Uint8)(255 * amount / 2);
            }
            else {
                right = (Uint8)(255 * amount);
                left = (Uint8)(255 * amount / 2);
            }

            Mix_SetPanning(
                    channel,
                    (Uint8)(left),
                    (Uint8)(right)
            );
        }
        else {
            return;
        }
    }

    Mix_PlayChannel(channel, sample->chunk_, 0);
}

void SamplePlayer::setEars(Ears *ears) {
    ears_ = ears;
}
