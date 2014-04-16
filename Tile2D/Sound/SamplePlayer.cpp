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
#include "SamplePlayer.h"
#include "Sample.h"
#include "Ears.h"

SamplePlayer::SamplePlayer() : Player() {
    ears_ = nullptr;
}

void SamplePlayer::play(Sample *sample, int channel, const Point* location) {
    if (!ears_ || location == nullptr) {
        Mix_SetPanning(channel, 255, 255);
    }
    else {
        double distance = location->distance(ears_->location->get());

        if (distance < ears_->maxDistance->get()) {
            double amount = 1.0 - (distance / ears_->maxDistance->get());

            if (amount < 0) {
                amount = 0;
            }

            double deltaX = location->x - ears_->location->get().x;

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
