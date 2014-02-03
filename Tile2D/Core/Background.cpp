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
#include "Background.h"
#include "Texture.h"

Background::Background(Texture *texture, int x, int y) {
    init(texture, x, y, texture->getW(), texture->getH());
}

Background::Background(Texture *texture, int x, int y, int w, int h) {
    init(texture, x, y, w, h);
}

void Background::init(Texture *texture, int x, int y, int w, int h) {
    this->texture = texture;
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
}

void Background::render(int offsetX, int offsetY, int w, int h) {
    texture->glBind();

    glColor3f(1, 1, 1);

    GLfloat textureLeftX = offsetX / (GLfloat)texture->getW();
    GLfloat textureRightX = (w + offsetX) / (GLfloat)texture->getW();
    GLfloat textureTopY = offsetY / (GLfloat)texture->getH();
    GLfloat textureBottomY = (h + offsetY) / (GLfloat)texture->getH();

    glBegin(GL_QUADS);
    glTexCoord2f(textureLeftX, textureTopY);
    glVertex2i(0, 0);
    glTexCoord2f(textureRightX, textureTopY);
    glVertex2i(w, 0);
    glTexCoord2f(textureRightX, textureBottomY);
    glVertex2i(w, h);
    glTexCoord2f(textureLeftX, textureBottomY);
    glVertex2i(0, h);
    glEnd();

    texture->glUnbind();
}

int Background::getX() {
    return x;
}

int Background::getY() {
    return y;
}

int Background::getW() {
    return w;
}

int Background::getH() {
    return h;
}
