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
#include "BasicDrawableBase.h"

BasicDrawableBase::BasicDrawableBase() :
isVisible_  (true       ),
opacity_    (1.0        ),
color_      (Color(1.0, 1.0, 1.0))
{

}

void BasicDrawableBase::draw(const Canvas& canvas) {
    if (!isVisible_) {
        return;
    }

    if (opacity_ < 1.0) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4d(color_.red, color_.green, color_.blue, opacity_);
    }
    else {
        glColor4d(color_.red, color_.green, color_.blue, 1.0);
    }

    glTranslated(transform()->getPosition().x, transform()->getPosition().y, 0);
    glRotatef(transform()->getRotation(), 0.0f, 0.0f, 1.0f);

    drawActual(canvas);

    glRotatef(-transform()->getRotation(), 0.0f, 0.0f, 1.0f);
    glTranslated(-transform()->getPosition().x, -transform()->getPosition().y, 0);

    if (opacity_ > 0.0 && opacity_ < 1.0) {
        glDisable(GL_BLEND);
    }
}

// getters and setters

bool BasicDrawableBase::isVisible() const {
    return isVisible_;
}

void BasicDrawableBase::setIsVisible(bool isVisible) {
    isVisible_ = isVisible;
}

float BasicDrawableBase::getOpacity() const {
    return opacity_;
}

void BasicDrawableBase::setOpacity(float opacity) {
    opacity_ = opacity;
}

const Color &BasicDrawableBase::getColor() const {
    return color_;
}

void BasicDrawableBase::setColor(const Color &color) {
    color_ = color;
}
