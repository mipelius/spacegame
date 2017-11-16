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

Background::Background() :
        // properties
        ratio(          Property<double>   (&ratio_)            ),
        texturePtr(     Property<Texture*> (&texturePtr_)       ),

        // values
        texturePtr_(nullptr),
        ratio_(1.0)
{

}

void Background::draw(const Canvas& canvas) {
    Rect rect = canvas.getCamera().areaRect.get();

    glColor3d(1.0, 1.0, 1.0);

    if (texturePtr_ != nullptr) {
        texturePtr_->glBind();

        Point location = canvas.getCamera().position.get();

        GLdouble x = (location.x * ratio_) / texturePtr_->getW();
        GLdouble y = (location.y * ratio_) / texturePtr_->getH();
        GLdouble w = rect.getWidth() / texturePtr_->getW();
        GLdouble h = rect.getHeight() / texturePtr_->getH();

        glBegin(GL_QUADS);
        glTexCoord2d(x - w / 2, y - h / 2);
        glVertex2d(rect.x1, rect.y1);
        glTexCoord2d(x + w / 2, y - h / 2);
        glVertex2d(rect.x2, rect.y1);
        glTexCoord2d(x + w / 2, y + h / 2);
        glVertex2d(rect.x2, rect.y2);
        glTexCoord2d(x - w / 2, y + h / 2);
        glVertex2d(rect.x1, rect.y2);
        glEnd();

        texturePtr_->glUnbind();
    }
    else {
        glBegin(GL_QUADS);
        glVertex2d(0, 0);
        glVertex2d(rect.getWidth(), 0);
        glVertex2d(rect.getWidth(), rect.getHeight());
        glVertex2d(0, rect.getHeight());
        glEnd();
    }
}