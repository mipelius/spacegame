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


#ifndef __Sprite_H_
#define __Sprite_H_

#include "Texture.h"
#include "DrawableBase.h"
#include "Rect.h"

class ColliderShape;
class ITexture;

class Sprite : public DrawableBase {

public:
    Sprite();

    // getters and setters

    const Rect &getRect() const;
    void setRect(const Rect &rect);
    ITexture *getTexturePtr() const;
    void setTexturePtr(ITexture *texturePtr);

protected:
    void drawActual(const Canvas &canvas) override;

private:
    Rect rect_;
    ITexture* texturePtr_;

    void drawRect_();
    void drawSpriteHavingTexture_();
};


#endif //__Sprite_H_
