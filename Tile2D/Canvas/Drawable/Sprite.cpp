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
#include "Sprite.h"

void Sprite::drawActual(const Canvas &canvas) {
    if (texturePtr_ == nullptr) {
        drawRect_();
    } else {
        drawSpriteHavingTexture_();
    }
}

Sprite::Sprite() :
        rect_({0, 0, 0, 0}),
        texturePtr_(nullptr)
{

}

// getters and setters

const Rect &Sprite::getRect() const {
    return rect_;
}

void Sprite::setRect(const Rect &rect) {
    rect_ = rect;
}

Texture *Sprite::getTexturePtr() const {
    return texturePtr_;
}

void Sprite::setTexturePtr(Texture *texturePtr) {
    texturePtr_ = texturePtr;
}

void Sprite::drawRect_() {
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);

    glVertex3f(rect_.x1, rect_.y1, 0.0);
    glVertex3f(rect_.x2, rect_.y1, 0.0);
    glVertex3f(rect_.x2, rect_.y2, 0.0);
    glVertex3f(rect_.x1, rect_.y2, 0.0);

    glEnd();
}

void Sprite::drawSpriteHavingTexture_() {
    texturePtr_->glBind();

    glBegin(GL_QUADS);

    glTexCoord2f(0.01, 0.01);
    glVertex3f(rect_.x1, rect_.y1, 0.0);
    glTexCoord2f(0.99, 0.01);
    glVertex3f(rect_.x2, rect_.y1, 0.0);
    glTexCoord2f(0.99, 0.99);
    glVertex3f(rect_.x2, rect_.y2, 0.0);
    glTexCoord2f(0.01, 0.99);
    glVertex3f(rect_.x1, rect_.y2, 0.0);

    glEnd();
}
