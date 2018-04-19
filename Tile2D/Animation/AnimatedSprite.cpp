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

#include "AnimatedSprite.h"

AnimatedSprite::AnimatedSprite() :
        animationPtr_(nullptr),
        currentFrame_(0),
        framesPerSecond_(60),
        loop_(false),
        isPlaying_(false),
        animationStartedTimestamp_(0)
{ }

void AnimatedSprite::drawActual(const Canvas &canvas) {
    if (isPlaying_) {
        Uint32 timePlayed = SDL_GetTicks() - animationStartedTimestamp_;
        currentFrame_ = (int)(timePlayed * 0.001f * framesPerSecond_);
        if (!loop_ && currentFrame_ >= animationPtr_->getFrames()) {
            stop();
        }
        else {
            currentFrame_ = currentFrame_ % animationPtr_->getFrames();
        }
    }

    const auto texturePtr = animationPtr_->getTexture();
    const auto texRect = animationPtr_->getTexCoords(currentFrame_);
    drawTexture_(texturePtr, texRect);
}

Animation *AnimatedSprite::getAnimationPtr() const {
    return animationPtr_;
}

void AnimatedSprite::setAnimationPtr(Animation *animationPtr) {
    animationPtr_ = animationPtr;
}

void AnimatedSprite::play(bool loop) {
    animationStartedTimestamp_ = SDL_GetTicks();
    loop_ = loop;
    isPlaying_ = true;
}

void AnimatedSprite::stop() {
    currentFrame_ = 0;
    isPlaying_ = false;
}

int AnimatedSprite::getFramesPerSecond() const {
    return framesPerSecond_;
}

void AnimatedSprite::setFramesPerSecond(int framesPerSecond) {
    framesPerSecond_ = framesPerSecond;
}

bool AnimatedSprite::isPlaying() const {
    return isPlaying_;
}
