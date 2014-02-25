// This file is part of SpaceGame.
// Copyright (C) 2014  Miika Pelkonen
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
#include "Background.h"
#include "Texture.h"
#include "Camera.h"
#include "Canvas.h"

Background::Background() {
    texture_ = nullptr;
    oldTexture_ = nullptr;
    ratio_ = 1.0;
}

void Background::setTexture(Texture *texture) {
    oldTexture_ = texture_;
    texture_ = texture;
    textureIsCrossFading_ = true;
    textureCrossFadingPhase_ = 0;
}

void Background::setRatio(double ratio) {
    ratio_ = ratio;
}

void Background::draw(Canvas* canvas) {
    if (textureIsCrossFading_) {
        double opacity = (double)textureCrossFadingPhase_ / TEXTURE_CROSSFADE_RENDER_COUNT;

        renderTexture(texture_, opacity, canvas);
        renderTexture(oldTexture_, 1.0 - opacity, canvas);

        textureCrossFadingPhase_++;
        if (textureCrossFadingPhase_ > TEXTURE_CROSSFADE_RENDER_COUNT) {
            textureCrossFadingPhase_ = 0;
            textureIsCrossFading_ = false;
        }
    }
    else renderTexture(texture_, 1.0, canvas);
}

void Background::renderTexture(Texture *texture, double opacity, Canvas* canvas) {
    if (!texture) return;

    texture->glBind();

    if (opacity < 1.0) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    glColor4f(1.0, 1.0, 1.0, (GLfloat)opacity);

    Rect rect = canvas->getCamera()->areaRect->get();
    Point location = canvas->getCamera()->location->get();

    GLdouble x = (location.x * ratio_) / texture->getW();
    GLdouble y = (location.y * ratio_) / texture->getH();
    GLdouble w = rect.getWidth() / texture->getW();
    GLdouble h = rect.getHeight() / texture->getH();

    glBegin(GL_QUADS);
    glTexCoord2d(x - w / 2, y - h / 2);
    glVertex2d(0, 0);
    glTexCoord2d(x + w / 2, y - h / 2);
    glVertex2d(rect.getWidth(), 0);
    glTexCoord2d(x + w / 2, y + h / 2);
    glVertex2d(rect.getWidth(), rect.getHeight());
    glTexCoord2d(x - w / 2, y + h / 2);
    glVertex2d(0, rect.getHeight());
    glEnd();

    if (opacity < 1.0) glDisable(GL_BLEND);

    texture->glUnbind();
}
