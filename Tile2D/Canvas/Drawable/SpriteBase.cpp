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

#include "SpriteBase.h"

SpriteBase::SpriteBase() :
        rect_({0, 0, 0, 0})
{

}

// getters and setters

const Rect &SpriteBase::getRect() const {
    return rect_;
}

void SpriteBase::setRect(const Rect &rect) {
    rect_ = rect;
}

void SpriteBase::drawTexture_(const Texture *texture, const Rect &texCoords) {
    if (texture != nullptr) {
        glEnable(GL_TEXTURE_2D);
        texture->glBind();
    } else {
        glDisable(GL_TEXTURE_2D);
    }

    glBegin(GL_QUADS);

    glTexCoord2f(texCoords.x1, texCoords.y1);
    glVertex3f(rect_.x1, rect_.y1, 0.0);
    glTexCoord2f(texCoords.x2, texCoords.y1);
    glVertex3f(rect_.x2, rect_.y1, 0.0);
    glTexCoord2f(texCoords.x2, texCoords.y2);
    glVertex3f(rect_.x2, rect_.y2, 0.0);
    glTexCoord2f(texCoords.x1, texCoords.y2);
    glVertex3f(rect_.x1, rect_.y2, 0.0);

    glEnd();
}
