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
#include "SpriteContainer.h"
#include "Camera.h"
#include "Sprite.h"
#include "SimpleProperty.h"
#include "Canvas.h"

SpriteContainer::SpriteContainer() : DrawableBase()
{

}

SpriteContainer::~SpriteContainer() {
    for (std::list<IDrawable*>::iterator i = sprites_.begin(); i != sprites_.end(); i++) {
        delete (*i);
    }
}

void SpriteContainer::drawActual(Canvas* canvas) {
    for (std::list<IDrawable*>::iterator i = sprites_.begin(); i != sprites_.end(); i++) {
        (*i)->draw(canvas);
    }
}

void SpriteContainer::addSprite(IDrawable* drawable) {
    sprites_.push_back(drawable);
}

void SpriteContainer::removeSprite(Sprite *sprite) {
    sprites_.remove(sprite);
}
