// This file is part of SpaceGame.
// Copyright (C) 2014 Miika Pelkonen
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
#include "DrawableGroup.h"

DrawableGroup::DrawableGroup() : DrawableBase()
{

}

DrawableGroup::~DrawableGroup() {
    for (std::list<IDrawable*>::iterator i = drawables_.begin(); i != drawables_.end(); i++) {
        delete (*i);
    }
}

void DrawableGroup::drawActual(Canvas* canvas) {
    for (std::list<IDrawable*>::iterator i = drawables_.begin(); i != drawables_.end(); i++) {
        (*i)->draw(canvas);
    }
}

void DrawableGroup::addDrawable(IDrawable *drawable) {
    drawables_.push_back(drawable);
}

void DrawableGroup::removeDrawable(IDrawable *drawable) {
    drawables_.remove(drawable);
}