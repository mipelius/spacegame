//
// Created by Miika Pelkonen on 1/10/14.
// Copyright (c) 2014 ___MIIKAPELKONEN___. All rights reserved.
//

#include "Background.h"



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

void Background::render(int offSetX, int offSetY, int w, int h) {
    texture->glBind();

    GLfloat textureLeftX = offSetX / (GLfloat)texture->getW();
    GLfloat textureRightX = (w + offSetX) / (GLfloat)texture->getW();
    GLfloat textureTopY = offSetY / (GLfloat)texture->getH();
    GLfloat textureBottomY = (h + offSetY) / (GLfloat)texture->getH();

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
