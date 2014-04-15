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

SpriteContainer::SpriteContainer() :
    location    (   new SimpleProperty<Point>   (&location_)    ),
    angle       (   new SimpleProperty<double>  (&angle_)       ),

    location_   (Point(0, 0)),
    angle_      (0          )
{

}

SpriteContainer::~SpriteContainer() {
    for (std::list<Sprite*>::iterator i = sprites_.begin(); i != sprites_.end(); i++) {
        delete (*i);
    }
}

void SpriteContainer::draw(Canvas* canvas) {
    Rect cameraRect = canvas->getCamera()->areaRect->get();

    for (std::list<Sprite*>::iterator i = sprites_.begin(); i != sprites_.end(); i++) {
        (*i)->render(Point(location_.x - cameraRect.x1, location_.y - cameraRect.y1), angle_);
    }
}

void SpriteContainer::addSprite(Sprite *sprite) {
    sprites_.push_back(sprite);
}