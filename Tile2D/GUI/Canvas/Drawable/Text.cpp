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
#include "Texture.h"

Text::Text() :
        fontPtr_(nullptr),
        fontSize_(1.0f),
        horizontalAlignment_(HorizontalAlignment::center),
        verticalAlignment_(VerticalAlignment::center)
{
    unsetSize_();
}


void Text::drawActual(const Canvas &canvas) {
    if (fontPtr_ == nullptr) {
        return;
    }

    if (size_.x < 0) {
        updateTextSize_();
    }

    fontPtr_->fontTexture_->glBind();

    float textureW = fontPtr_->fontTexture_->getW();
    float textureH = fontPtr_->fontTexture_->getH();

    float offsetX;
    switch (horizontalAlignment_) {
        case HorizontalAlignment::left : {
            offsetX = 0.0f;
            break;
        }
        case HorizontalAlignment::center : {
            offsetX = size_.x / -2.0f;
            break;
        }
        case HorizontalAlignment::right : {
            offsetX = -size_.x;
            break;
        }
    }

    float offsetY;
    switch (verticalAlignment_) {
        case VerticalAlignment::top : {
            offsetY = 0.0f;
            break;
        }
        case VerticalAlignment::center : {
            offsetY = size_.y / -2.0f;
            break;
        }
        case VerticalAlignment::bottom : {
            offsetY = -size_.y;
            break;
        }
    }

    glBegin(GL_QUADS);

    for (int i = 0; i < string_.length(); i++) {
        char ch = string_.data()[i];
        Font::Letter* letter = fontPtr_->getLetter(ch);

        if (letter) {
            float texX1 = (1.0f / textureW) * letter->x;
            float x1 = 0;
            float texY1 = (1.0f / textureH) * letter->y;
            float y1 = 0;

            float texX2 = (1.0f / textureW) * (letter->x + letter->w);
            float x2 = (0 + letter->w) * fontSize_;
            float texY2 = (1.0f / textureH) * letter->y;
            float y2 = 0;

            float texX3 = (1.0f / textureW) * (letter->x + letter->w);
            float x3 = (0 + letter->w) * fontSize_;
            float texY3 = (1.0f / textureH) * (letter->y + letter->h);
            float y3 = (0 + letter->h) * fontSize_;

            float texX4 = (1.0f / textureW) * letter->x;
            float x4 = 0;
            float texY4 = (1.0f / textureH) * (letter->y + letter->h);
            float y4 = (0 + letter->h) * fontSize_;

            x1 += offsetX; x2 += offsetX; x3 += offsetX; x4 += offsetX;
            y1 += offsetY; y2 += offsetY; y3 += offsetY; y4 += offsetY;

            glTexCoord2f(texX1, texY1); glVertex2f(x1, y1);
            glTexCoord2f(texX2, texY2); glVertex2f(x2, y2);
            glTexCoord2f(texX3, texY3); glVertex2f(x3, y3);
            glTexCoord2f(texX4, texY4); glVertex2f(x4, y4);

            offsetX += (letter->w) * fontSize_;
        }
    }

    glEnd();

    fontPtr_->fontTexture_->glUnbind();
}

// getters and setters

Font *Text::getFontPtr() const {
    return fontPtr_;
}

void Text::setFontPtr(Font *fontPtr) {
    fontPtr_ = fontPtr;
    unsetSize_();
}

float Text::getFontSize() const {
    return fontSize_;
}

void Text::setFontSize(float size) {
    fontSize_ = size;
    unsetSize_();
}

const std::string &Text::getString() const {
    return string_;
}

void Text::setString(const std::string &string) {
    string_ = string;
    unsetSize_();
}

void Text::updateTextSize_() {
    size_ = {0, 0};
    for (int i = 0; i < string_.length(); i++) {
        char ch = string_.data()[i];
        Font::Letter* letter = fontPtr_->getLetter(ch);

        if (letter) {
            size_.x += letter->w * fontSize_;
            float letterH = letter->h * fontSize_;
            if (letterH > size_.y) {
                size_.y = letterH;
            }
        }
    }
}

void Text::unsetSize_() {
    size_ = {-1.0f, -1.0f};
}

Text::HorizontalAlignment Text::getHorizontalAlignment() const {
    return horizontalAlignment_;
}

void Text::setHorizontalAlignment(Text::HorizontalAlignment horizontalAlignment) {
    horizontalAlignment_ = horizontalAlignment;
}

Text::VerticalAlignment Text::getVerticalAlignment() const {
    return verticalAlignment_;
}

void Text::setVerticalAlignment(Text::VerticalAlignment verticalAlignment) {
    verticalAlignment_ = verticalAlignment;
}

