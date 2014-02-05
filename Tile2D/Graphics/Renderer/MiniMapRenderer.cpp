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
#include "MiniMapRenderer.h"
#include "Camera.h"
#include "Map.h"

MiniMapRenderer::MiniMapRenderer(Rect const &renderingAreaRect) : MapRendererBase(renderingAreaRect) { }

void MiniMapRenderer::render() {
    if (!map_) return;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    double opacity = 0.4;

    double stepX = camera_->getRect().getWidth() / renderingAreaRect_.getWidth();
    double stepY = camera_->getRect().getHeight() / renderingAreaRect_.getHeight();

    glColor4f(0.0, 0.0, 0.0, opacity);

    glRectf(
            (GLfloat)renderingAreaRect_.x1,
            (GLfloat)renderingAreaRect_.y1,
            (GLfloat)renderingAreaRect_.x2,
            (GLfloat)renderingAreaRect_.y2
    );

    glBegin(GL_POINTS);
    int x = (int)renderingAreaRect_.x1;
    for (double mapX = camera_->getRect().x1; mapX <= camera_->getRect().x2; mapX += stepX) {
        int y = (int)renderingAreaRect_.y1;
        for (double mapY = camera_->getRect().y1; mapY <= camera_->getRect().y2; mapY += stepY) {
            unsigned char value = map_->getValueActual((int)mapX, (int)mapY);

            if (value != 0) {
                if (value % 3 == 2) glColor4f(0.7, 0.0, 0.0, opacity); // red
                if (value % 3 == 1) glColor4f(0.0, 0.7, 0.0, opacity); // green
                if (value % 3 == 0) glColor4f(0.0, 0.0, 0.7, opacity); // blue

                glVertex2i(x, y);
            }
            y++;
        }
        x++;
    }
    glEnd();

    glColor4f(0.6, 0.6, 1.0, opacity);

    glBegin(GL_LINE_LOOP);
    glVertex2d(renderingAreaRect_.x1, renderingAreaRect_.y1 - 1);
    glVertex2d(renderingAreaRect_.x2 + 1, renderingAreaRect_.y1 - 1);
    glVertex2d(renderingAreaRect_.x2 + 1, renderingAreaRect_.y2);
    glVertex2d(renderingAreaRect_.x1, renderingAreaRect_.y2);
    glEnd();

    glDisable(GL_BLEND);

    glColor3f(1.0, 1.0, 1.0);

    Point center = Point(
            renderingAreaRect_.x1 + renderingAreaRect_.getWidth() / 2,
            renderingAreaRect_.y1 + renderingAreaRect_.getHeight() / 2
    );

    glBegin(GL_POINTS);
    glVertex2i(center.x, center.y);
    glVertex2i((int)center.x - 1, center.y);
    glVertex2i((int)center.x + 1, center.y);
    glVertex2i((int)center.x, (int)center.y - 1);
    glVertex2i((int)center.x, (int)center.y + 1);
    glEnd();
}
