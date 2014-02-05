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
#include "GameObject.h"
#include "Texture.h"
#include "AnimatedTexture.h"

GameObject::GameObject(
        Point location,
        double angle,
        ITexture *texture,
        double mass,
        int w,
        int h,
        CollisionShape *collisionShape
) : GameEntity(location, angle, collisionShape) {
    this->texture = texture;
    mass == 0 ? this->mass = ZERO_MASS : this->mass = mass;
    this->w = w;
    this->h = h;
}

void GameObject::render(double x, double y) {
    GameEntity::render(x, y);
    glTranslatef(x, y, 0);
    glRotatef((GLfloat)angle, 0.0f, 0.0f, 1.0f);
    glTranslatef(-x, -y, 0);

    glColor3f(1.0, 1.0, 1.0);
    texture->glBind();
    glBegin(GL_QUADS);

    texture->glTexCorner(Texture::Corner::TOP_LEFT);
    glVertex3f((GLfloat)(x),(GLfloat)(y), 0.0);

    texture->glTexCorner(Texture::Corner::TOP_RIGHT);
    glVertex3f((GLfloat)(x + w),(GLfloat)(y), 0.0);

    texture->glTexCorner(Texture::Corner::BOTTOM_LEFT);
    glVertex3f((GLfloat)(x + w),(GLfloat)(y + h), 0.0);

    texture->glTexCorner(Texture::Corner::BOTTOM_RIGHT);
    glVertex3f((GLfloat)(x),(GLfloat)(y + h), 0.0);

    glEnd();
    texture->glUnbind();

    glTranslatef(x, y, 0);
    glRotatef((GLfloat)-angle, 0.0f, 0.0f, 1.0f);
    glTranslatef(-x, -y, 0);
}

double GameObject::getMass() {
    return mass;
}

void GameObject::update(double timeElapsedSec) {
    // if the texture is animatedTexture, update it

    AnimatedTexture* animatedTexture = dynamic_cast<AnimatedTexture*>(texture);

    if (animatedTexture) {
        animatedTexture->update(timeElapsedSec);
    }
}
