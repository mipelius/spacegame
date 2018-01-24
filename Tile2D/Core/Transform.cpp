// This file is part of SpaceGame.
// Copyright (C) 2018 Miika Pelkonen
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

#include "Transform.h"
#include "precompile.h"

// getters and setters

const Vecf &Transform::getPosition() const {
    return position_;
}

void Transform::setPosition(const Vecf &position) {
    position_ = position;
}

float Transform::getRotation() const {
    return rotation_;
}

void Transform::setRotation(float rotation) {
    rotation_ = rotation;
}

Transform::Transform() {

}

void Transform::glTransform() {
    glTranslatef(position_.x, position_.y, 0);
    glRotatef(rotation_, 0.0f, 0.0f, 1.0f);
    glScalef(scale_.x, scale_.y, 1.0f);
}

void Transform::glInvTransform() {
    glScalef(1.0f / scale_.x, 1.0f / scale_.y, 1.0f);
    glRotatef(-rotation_, 0.0f, 0.0f, 1.0f);
    glTranslatef(-position_.x, -position_.y, 0);
}

const Vecf &Transform::getScale() const {
    return scale_;
}

void Transform::setScale(const Vecf &scale) {
    scale_ = scale;
}
