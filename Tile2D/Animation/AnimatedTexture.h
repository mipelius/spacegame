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


#ifndef __AnimatedTexture_H_
#define __AnimatedTexture_H_

#include "Texture.h"
#include "ITexture.h"
#include "AnimationBase.h"

struct AnimationStoppedEventArgs { };

class AnimatedTexture : public AnimationBase, public ITexture {
private:
    Texture* texture_;

public:
    AnimatedTexture(unsigned int framesPerSecond, unsigned int frameAmount, bool enableLoop, Texture* texture);
    void updateActual() override;

    void glBind() override ;
    void glTexCorner(Corner corner) override;
    void glUnbind() override;

    Event<AnimatedTexture, AnimationStoppedEventArgs> const stopped;

    void stop() override ;
};

#endif //__AnimatedTexture_H_
