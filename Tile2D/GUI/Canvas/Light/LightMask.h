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

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "IDrawable.h"
#include "Canvas.h"

class LightMask : public IDrawable {

public:
    LightMask(int w, int h);
    ~LightMask();

    Property<double>* const ambientLight;

    void add(ILight *light);

    void addAlpha(int x, int y, double value);

protected:
    void draw(Canvas *canvas);

private:
    std::list<ILight*> lights;

    double ambientLight_;
    int w_;
    int h_;

    GLuint glTextureId_;
    Uint32* pixels_;

    void initialize();
    void clearPixels();

    Uint8 getNormalizedAmbientLightByte();

    GLint lightMaskTextureId_;
    bool lightMaskTextureCreated_;
};

#endif //__LightMask_H_
