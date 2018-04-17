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


#include "Camera.h"

Camera::Camera() :
    // member objects
    areaRect_(      Rect(0, 0, 0, 0)    )
{

}

void Camera::zoom(float amount) {
    float ratio = areaRect_.getHeight() / areaRect_.getWidth();

    areaRect_.x1 += amount;
    areaRect_.y1 += amount * ratio;
    areaRect_.x2 -= amount;
    areaRect_.y2 -= amount * ratio;
}

// getters and setters

Vecf Camera::getPosition() {
    float x = areaRect_.x1 + areaRect_.getWidth() / 2;
    float y = areaRect_.y1 + areaRect_.getHeight() / 2;

    return Vecf(x, y);
}

void Camera::setPosition(const Vecf &value) {
    float w = areaRect_.getWidth();
    float h = areaRect_.getHeight();

    areaRect_.x1 = value.x - w / 2;
    areaRect_.y1 = value.y - h / 2;
    areaRect_.x2 = areaRect_.x1 + w;
    areaRect_.y2 = areaRect_.y1 + h;
}

const Rect &Camera::getAreaRect() const {
    return areaRect_;
}

void Camera::setAreaRect(const Rect &areaRect) {
    areaRect_ = areaRect;
}
