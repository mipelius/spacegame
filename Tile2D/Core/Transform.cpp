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


#include "Transform.h"

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

void Transform::deserialize(const json::Object &jsonObject) {
    if (jsonObject.HasKey("scale")) {
        Vecf scale;
        scale.deserialize(jsonObject["scale"]);
        setScale(scale);
    }
    if (jsonObject.HasKey("position")) {
        Vecf position;
        position.deserialize(jsonObject["position"]);
        setPosition(position);
    }
    if (jsonObject.HasKey("rotation")) {
        setRotation(jsonObject["rotation"].ToFloat());
    }

}