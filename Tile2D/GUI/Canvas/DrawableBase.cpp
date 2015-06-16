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
#include "Canvas.h"
#include "Camera.h"
#include <SDL2/SDL_opengl.h>

DrawableBase::DrawableBase() :

// properties
location    (   new SimpleProperty<Point>   (&location_)    ),
angle       (   new SimpleProperty<double>  (&angle_)       ),
isVisible   (   new SimpleBooleanProperty   (&isVisible_)   ),

// private attributes initialization
location_   (Point(0, 0)),
angle_      (0          ),
isVisible_  (true       )

{

}

DrawableBase::~DrawableBase() {
    delete location;
    delete angle;
    delete isVisible;
}

void DrawableBase::draw(Canvas *canvas) {
    if (!isVisible_) {
        return;
    }

    glTranslated(location_.x, location_.y, 0);
    glRotatef((GLfloat)angle_, 0.0f, 0.0f, 1.0f);

    drawActual(canvas);

    glRotatef((GLfloat)-angle_, 0.0f, 0.0f, 1.0f);
    glTranslated(-location_.x, -location_.y, 0);
}
