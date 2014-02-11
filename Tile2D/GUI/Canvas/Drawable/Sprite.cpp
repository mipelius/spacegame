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
#include "AnimatedTexture.h"

Sprite::Sprite(ITexture *texture, Rect rect) : rect_(rect) {
    texture_ = texture;
}

void Sprite::render(Point locationPoint, double angle) {
    double x = locationPoint.x;
    double y = locationPoint.y;

    glTranslated(x, y, 0);
    glRotatef((GLfloat)angle, 0.0f, 0.0f, 1.0f);
    glTranslated(-x, -y, 0);

    glColor3f(1.0, 1.0, 1.0);

    texture_->glBind();

    glBegin(GL_QUADS);

    texture_->glTexCorner(Texture::Corner::TOP_LEFT);
    glVertex3d(x + rect_.x1, y + rect_.y1, 0.0);

    texture_->glTexCorner(Texture::Corner::TOP_RIGHT);
    glVertex3d(x + rect_.x2, y + rect_.y1, 0.0);

    texture_->glTexCorner(Texture::Corner::BOTTOM_RIGHT);
    glVertex3d(x + rect_.x2, y + rect_.y2, 0.0);

    texture_->glTexCorner(Texture::Corner::BOTTOM_LEFT);
    glVertex3d(x + rect_.x1, y + rect_.y2, 0.0);

    glEnd();

    glTranslated(x, y, 0);
    glRotatef((GLfloat)-angle, 0.0f, 0.0f, 1.0f);
    glTranslated(-x, -y, 0);

    texture_->glUnbind();
}