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

#include "DrawableBase.h"

DrawableBase::DrawableBase() :

// properties
position    (   Property<Point>   (&position_)    ),
angle       (   Property<double>  (&angle_)       ),
isVisible   (   BooleanProperty   (&isVisible_)   ),
opacity     (   Property<double>  (&opacity_)     ),
color       (   Property<Color>   (&color_)       ),

// private attributes initialization
position_   (Point(0, 0)),
angle_      (0          ),
isVisible_  (true       ),
opacity_    (1.0        ),
color_      (Color(1.0, 1.0, 1.0))

{

}

void DrawableBase::draw(const Canvas& canvas) {
    if (!isVisible_) {
        return;
    }

    if (opacity_ > 0.0 && opacity_ < 1.0) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4d(color_.red, color_.green, color_.blue, opacity_);
    }
    else {
        glColor4d(color_.red, color_.green, color_.blue, 1.0);
    }

    glTranslated(position_.x, position_.y, 0);
    glRotatef((GLfloat)angle_, 0.0f, 0.0f, 1.0f);

    drawActual(canvas);

    glRotatef((GLfloat)-angle_, 0.0f, 0.0f, 1.0f);
    glTranslated(-position_.x, -position_.y, 0);

    if (opacity_ > 0.0 && opacity_ < 1.0) {
        glDisable(GL_BLEND);
    }
}
