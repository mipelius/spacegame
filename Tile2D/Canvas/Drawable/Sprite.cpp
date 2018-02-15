// This file is part of SpaceGame.
// Copyright (C) 2014  Miika Pelkonen
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

ITexture *Sprite::getTexturePtr() const {
    return texturePtr_;
}

void Sprite::setTexturePtr(ITexture *texturePtr) {
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

    texturePtr_->glTexCorner(Texture::Corner::TOP_LEFT);
    glVertex3f(rect_.x1, rect_.y1, 0.0);

    texturePtr_->glTexCorner(Texture::Corner::TOP_RIGHT);
    glVertex3f(rect_.x2, rect_.y1, 0.0);

    texturePtr_->glTexCorner(Texture::Corner::BOTTOM_RIGHT);
    glVertex3f(rect_.x2, rect_.y2, 0.0);

    texturePtr_->glTexCorner(Texture::Corner::BOTTOM_LEFT);
    glVertex3f(rect_.x1, rect_.y2, 0.0);

    glEnd();

    texturePtr_->glUnbind();
}
