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

class IDrawable;
class LightMask;

class Camera;

#include "GuiComponentBase.h"

class Canvas : public GuiComponentBase {

public:
    Canvas();
    ~Canvas();

    void renderActual();
    void addDrawable(IDrawable *drawable);

    void addLightMask(LightMask* lightMask);

    void setCamera(Camera* camera);
    Camera* getCamera();

private:
    std::list<IDrawable *> drawables_;
    std::list<LightMask *> lightMasks_;
    Camera* camera_;
};

#endif //__Canvas_H_
