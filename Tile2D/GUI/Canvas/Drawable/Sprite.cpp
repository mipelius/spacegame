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
#include "Texture.h"

Sprite::Sprite(ITexture *texture, Rect rect) : DrawableBase(), rect_(rect) {
    texture_ = texture;
}

void Sprite::drawActual(const Canvas &canvas) {
    texture_->glBind();

    glBegin(GL_QUADS);

    texture_->glTexCorner(Texture::Corner::TOP_LEFT);
    glVertex3f(rect_.x1, rect_.y1, 0.0);

    texture_->glTexCorner(Texture::Corner::TOP_RIGHT);
    glVertex3f(rect_.x2, rect_.y1, 0.0);

    texture_->glTexCorner(Texture::Corner::BOTTOM_RIGHT);
    glVertex3f(rect_.x2, rect_.y2, 0.0);

    texture_->glTexCorner(Texture::Corner::BOTTOM_LEFT);
    glVertex3f(rect_.x1, rect_.y2, 0.0);

    glEnd();

    texture_->glUnbind();
}

Rect Sprite::getRect() {
    return rect_;
}

Rect Sprite::getActualRect() {
    double x = location.get().x;
    double y = location.get().y;

    return {
            rect_.x1 + x,
            rect_.y1 + y,
            rect_.x2 + x,
            rect_.y2 + y
    };
}

Sprite::Sprite() : rect_(Rect(0, 0, 0, 0)) {

}
