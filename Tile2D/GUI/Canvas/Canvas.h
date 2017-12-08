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

#ifndef __Canvas_H_
#define __Canvas_H_

class DrawableBase;
class IShadowMask;
class Camera;

#include "GuiComponentBase.h"

class Canvas : public GuiComponentBase {
    friend class Tile2D;
    friend class DrawableBase;

public:
    void setCamera(Camera* camera);
    Camera& getCamera() const;

private:
    Canvas();

    void renderActual() override;
    void addDrawable(DrawableBase *drawable);
    void removeDrawable(DrawableBase* drawable);

    void addShadowMask(IShadowMask *shadowMask);

    std::list<DrawableBase*> drawables_;
    std::list<IShadowMask*> shadowMasks_;
    Camera* camera_;
};

#endif //__Canvas_H_
