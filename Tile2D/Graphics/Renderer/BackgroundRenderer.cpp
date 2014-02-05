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
#include "BackgroundRenderer.h"
#include "Texture.h"
#include "Camera.h"

BackgroundRenderer::BackgroundRenderer(Rect renderingAreaRect) : RendererBase(renderingAreaRect) {
    texture_ = nullptr;
    oldTexture_ = nullptr;
    ratio_ = 1.0;
}

void BackgroundRenderer::setBackgroundTexture(Texture* texture) {
    oldTexture_ = texture_;
    texture_ = texture;
    textureIsCrossFading_ = true;
    textureCrossFadingPhase_ = 0;
}

void BackgroundRenderer::setRatio(double ratio) {
    ratio_ = ratio;
}

void BackgroundRenderer::render() {
    if (textureIsCrossFading_) {
        double opacity = (double)textureCrossFadingPhase_ / TEXTURE_CROSSFADE_RENDER_COUNT;

        renderTexture(texture_, opacity);
        renderTexture(oldTexture_, 1.0 - opacity);

        textureCrossFadingPhase_++;
        if (textureCrossFadingPhase_ > TEXTURE_CROSSFADE_RENDER_COUNT) {
            textureCrossFadingPhase_ = 0;
            textureIsCrossFading_ = false;
        }
    }
    else renderTexture(texture_, 1.0);
}

void BackgroundRenderer::renderTexture(Texture *texture, double opacity) {
    if (!texture) return;

    texture->glBind();

    if (opacity < 1.0) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    glColor4f(1.0, 1.0, 1.0, (GLfloat)opacity);

    Rect rect = camera_->getRect();

    GLfloat x = (GLfloat)(rect.x1 * ratio_) / texture->getW();
    GLfloat y = (GLfloat)(rect.y1 * ratio_) / texture->getH();
    GLfloat w = (GLfloat)rect.getWidth() / texture->getW();
    GLfloat h = (GLfloat)rect.getHeight() / texture->getH();

    glBegin(GL_QUADS);
    glTexCoord2f(x, y);
    glVertex2i((GLint)renderingAreaRect_.x1, (GLint)renderingAreaRect_.y1);
    glTexCoord2f(x + w, y);
    glVertex2i((GLint)renderingAreaRect_.x2, (GLint)renderingAreaRect_.y1);
    glTexCoord2f(x + w, y + h);
    glVertex2i((GLint)renderingAreaRect_.x2, (GLint)renderingAreaRect_.y2);
    glTexCoord2f(x, y + h);
    glVertex2i((GLint)renderingAreaRect_.x1, (GLint)renderingAreaRect_.y2);
    glEnd();

    if (opacity < 1.0) glDisable(GL_BLEND);

    texture->glUnbind();
}
