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

#ifndef __LightMask_H_
#define __LightMask_H_

class ILight;
class Canvas;

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "Property.h"

class LightMask {
    friend class Canvas;

public:
    LightMask(double w, double h);
    ~LightMask();

    Property<double>* const ambientLight;

    void add(ILight *light);

protected:
    void draw(Canvas *canvas);
    void update(Canvas *canvas);

private:
    std::list<ILight*> lights;

    double ambientLight_;
    double w_;
    double h_;

    GLuint glTextureId_;

    void initialize();

    GLint lightMaskTextureId_;
};

#endif //__LightMask_H_
