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

#include "LightObject.h"
#include "Texture.h"
#include "Sprite.h"
#include "SpriteContainer.h"
#include "PointLight.h"

Texture* lightTexture = nullptr;

LightObject::LightObject(Point location, double radius) :
    spriteContainer(new SpriteContainer()),
    pointLight(new PointLight(location, radius))
{
    if (!lightTexture) {
        lightTexture = new Texture("images/light.png");
    }

    Sprite* sprite = new Sprite(lightTexture, Rect(-64, -64, 64, 64));

    spriteContainer->addSprite(sprite);
    spriteContainer->location->set(location);
}
