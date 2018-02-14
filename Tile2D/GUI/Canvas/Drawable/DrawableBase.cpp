// This file is part of SpaceGame.
// Copyright (C) 2015 Miika Pelkonen
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

#include "Tile2D.h"
#include "DrawableBase.h"

DrawableBase::DrawableBase() :
isVisible_      (true       ),
opacity_        (1.0        ),
color_          (Color(1.0, 1.0, 1.0)),
sortingLayer_   (0)
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
    drawActual(canvas);
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