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

#ifndef SPACEGAME_ANIMATEDSPRITE_H
#define SPACEGAME_ANIMATEDSPRITE_H


#include "SpriteBase.h"
#include "Animation.h"

class AnimatedSprite : public SpriteBase {
private:
    Animation* animationPtr_;
    int currentFrame_;
    int framesPerSecond_;
    bool loop_;
    bool isPlaying_;
    Uint32 animationStartedTimestamp_;

public:
    AnimatedSprite();

    Animation *getAnimationPtr() const;
    void setAnimationPtr(Animation *animationPtr);
    int getFramesPerSecond() const;
    void setFramesPerSecond(int framesPerSecond);
    bool isPlaying() const;

    void play(bool loop = true);
    void stop();

protected:
    void drawActual(const Canvas &canvas) override;
};


#endif //SPACEGAME_ANIMATEDSPRITE_H
