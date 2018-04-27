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


#include "Tile2D.h"
#include "DrawableBase.h"

DrawableBase::DrawableBase() :
isVisible_      (true       ),
opacity_        (1.0        ),
color_          (Color(1.0, 1.0, 1.0)),
sortingLayer_   (0),
isUIDrawable_   (false      )
{

}

void DrawableBase::init() {
    Tile2D::canvas().addDrawable(this);
}

void DrawableBase::onDestroy() {
    Tile2D::canvas().removeDrawable(this);
}

void DrawableBase::draw(const Canvas& canvas) {
    if (!isVisible_) {
        return;
    }
    if (opacity_ <= 0.0) {
        return;
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(color_.red, color_.green, color_.blue, opacity_);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    transform()->glTransform();
    localTransform_.glTransform();
    drawActual(canvas);
    localTransform_.glInvTransform();
    transform()->glInvTransform();
}

// getters and setters

bool DrawableBase::isVisible() const {
    return isVisible_;
}

void DrawableBase::setIsVisible(bool isVisible) {
    isVisible_ = isVisible;
}

float DrawableBase::getOpacity() const {
    return opacity_;
}

void DrawableBase::setOpacity(float opacity) {
    opacity_ = opacity;
}

const Color &DrawableBase::getColor() const {
    return color_;
}

void DrawableBase::setColor(const Color &color) {
    color_ = color;
}

int DrawableBase::getSortingLayer() const {
    return sortingLayer_;
}

void DrawableBase::setSortingLayer(int sortingLayer) {
    sortingLayer_ = sortingLayer;
}

bool DrawableBase::isUIDrawable() const {
    return isUIDrawable_;
}

void DrawableBase::setIsUIDrawable(bool isUIDrawable) {
    isUIDrawable_ = isUIDrawable;
}

Transform &DrawableBase::localTransform() {
    return localTransform_;
}
