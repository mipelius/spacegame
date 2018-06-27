// MIT License
//
// This file is part of SpaceGame.
// Copyright (c) 2014-2018 Miika Pelkonen
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


#include "Tile2D.h"
#include "Resources.h"
#include "precompile.h"
#include "Background.h"
#include "Texture.h"
#include "Camera.h"
#include "Canvas.h"

Background::Background() :
        // values
        texturePtr_(nullptr),
        ratio_(1.0)
{

}

void Background::drawActual(const Canvas& canvas) {
    Rect rect = canvas.getCamera()->getAreaRect();

    if (texturePtr_ != nullptr) {
        glEnable(GL_TEXTURE_2D);
        texturePtr_->glBind();

        Vecf position = canvas.getCamera()->getPosition();

        GLfloat x = (position.x * ratio_) / texturePtr_->getW();
        GLfloat y = (position.y * ratio_) / texturePtr_->getH();
        GLfloat w = rect.getWidth() / texturePtr_->getW();
        GLfloat h = rect.getHeight() / texturePtr_->getH();

        glBegin(GL_QUADS);
        glTexCoord2f(x - w / 2, y - h / 2);
        glVertex2f(rect.x1, rect.y1);
        glTexCoord2f(x + w / 2, y - h / 2);
        glVertex2f(rect.x2, rect.y1);
        glTexCoord2f(x + w / 2, y + h / 2);
        glVertex2f(rect.x2, rect.y2);
        glTexCoord2f(x - w / 2, y + h / 2);
        glVertex2f(rect.x1, rect.y2);
        glEnd();

    }
    else {
        glBegin(GL_QUADS);
        glVertex2f(0, 0);
        glVertex2f(rect.getWidth(), 0);
        glVertex2f(rect.getWidth(), rect.getHeight());
        glVertex2f(0, rect.getHeight());
        glEnd();
    }
}

// getters and setters

Texture *Background::getTexturePtr() const {
    return texturePtr_;
}

void Background::setTexturePtr(Texture *texturePtr) {
    texturePtr_ = texturePtr;
}

float Background::getRatio() const {
    return ratio_;
}

void Background::setRatio(float ratio) {
    ratio_ = ratio;
}

Tile2DComponent *Background::clone() {
    return new Background(*this);
}

void Background::deserialize(const json::Object &jsonObject) {
    DrawableBase::deserialize(jsonObject);

    if (jsonObject.HasKey("texture")) {
        auto textureName = jsonObject["texture"].ToString();
        texturePtr_ = Tile2D::resources().textures[textureName];
    }
    if (jsonObject.HasKey("ratio")) {
        ratio_ = jsonObject["ratio"].ToFloat();
    }
}
