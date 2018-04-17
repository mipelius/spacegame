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
#include "AnimatedTexture.h"

AnimatedTexture::AnimatedTexture(unsigned int framesPerSecond, unsigned int frameAmount, bool enableLoop, Texture* texture) :
    AnimationBase(framesPerSecond, frameAmount, enableLoop),
    stopped   (   Event<AnimatedTexture, AnimationStoppedEventArgs>(this) )
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
    stopped.raise(AnimationStoppedEventArgs());
}
