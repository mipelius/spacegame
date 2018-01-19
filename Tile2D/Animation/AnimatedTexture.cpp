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
#include "AnimatedTexture.h"

AnimatedTexture::AnimatedTexture(unsigned int framesPerSecond, unsigned int frameAmount, bool enableLoop, Texture* texture) :
    AnimationBase(framesPerSecond, frameAmount, enableLoop),
    stopped   (   Event<AnimatedTexture, EventArgs>(this) )
{
    texture_ = texture;
}

void AnimatedTexture::glTexCorner(ITexture::Corner corner) {
    GLfloat w = 1.0f / frameAmount_;
    GLfloat left = currentFrame_ * w + 0.01f;
    GLfloat right = (currentFrame_ + 1) * w - 0.01f;

    switch (corner) {
        case (TOP_LEFT):
            glTexCoord2f(left, 0.01);
            break;
        case (TOP_RIGHT):
            glTexCoord2f(right, 0.01);
            break;
        case (BOTTOM_LEFT):
            glTexCoord2f(left, 0.99);
            break;
        case (BOTTOM_RIGHT):
            glTexCoord2f(right, 0.99);
            break;
    }
}

void AnimatedTexture::glUnbind() {
    this->texture_->glUnbind();
}

void AnimatedTexture::glBind() {
    this->texture_->glBind();
}

void AnimatedTexture::updateActual() {
    // do nothing
}

void AnimatedTexture::stop() {
    AnimationBase::stop();
    stopped.raise(EventArgs());
}
