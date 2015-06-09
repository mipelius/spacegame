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

#include <iostream>
#include "Text.h"
#include "Font.h"
#include "SimpleProperty.h"
#include "Canvas.h"
#include "Camera.h"
#include "Texture.h"

Text::Text(Font *font) :

    location(   new SimpleProperty<Point>       (&location_)    ),
    size(       new SimpleProperty<double>      (&size_   )     ),
    string(     new SimpleProperty<std::string> (&string_)   ),

    location_(Point(0,0))
{
    font_ = font;
}

Text::~Text() {

}

void Text::draw(Canvas *canvas) {
    font_->fontTexture_->glBind();

    float textureW = font_->fontTexture_->getW();
    float textureH = font_->fontTexture_->getH();

    float offsetX = 0;
    float offsetY = 0;

    glBegin(GL_QUADS);

    for (int i = 0; i < string_.length(); i++) {
        unsigned char ch = string_.data()[i];
        Font::Letter* letter = font_->getLetter(ch);

        if (letter) {
            float texX1 = (1.0 / textureW) * letter->x;
            float x1 = 0;
            float texY1 = (1.0 / textureH) * letter->y;
            float y1 = 0;

            float texX2 = (1.0 / textureW) * (letter->x + letter->w);
            float x2 = 0 + letter->w;
            float texY2 = (1.0 / textureH) * letter->y;
            float y2 = 0;

            float texX3 = (1.0 / textureW) * (letter->x + letter->w);
            float x3 = 0 + letter->w;
            float texY3 = (1.0 / textureH) * (letter->y + letter->h);
            float y3 = 0 + letter->h;

            float texX4 = (1.0 / textureW) * letter->x;
            float x4 = 0;
            float texY4 = (1.0 / textureH) * (letter->y + letter->h);
            float y4 = 0 + letter->h;

            x1 += offsetX; x2 += offsetX; x3 += offsetX; x4 += offsetX;
            y1 += offsetY; y2 += offsetY; y3 += offsetY; y4 += offsetY;

            glTexCoord2f(texX1, texY1); glVertex2f(x1, y1);
            glTexCoord2f(texX2, texY2); glVertex2f(x2, y2);
            glTexCoord2f(texX3, texY3); glVertex2f(x3, y3);
            glTexCoord2f(texX4, texY4); glVertex2f(x4, y4);

            offsetX += letter->w;
            // offsetY = ???
        }
        else {
//            printf("\"%c\"", ch);
        }

    }

    glEnd();

    font_->fontTexture_->glUnbind();
}
