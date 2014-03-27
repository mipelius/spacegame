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

#ifndef __MapTexture_H_
#define __MapTexture_H_

#include <SDL2/SDL_opengl.h>

class MapTexture {
private:
    static const int GL_TEXTURE_SIZE = 1024; // Width == Height

    GLuint glTextureId_;
    int blockW_;
    int blockH_;

    int blockInternalSize_;

    int count;

    void createGlTexture();

public:
    static const unsigned int CORNER_ROUNDING_NONE           =   0;         // 00000000
    static const unsigned int CORNER_ROUNDING_TOP_LEFT       =   1 << 0;    // 00000001
    static const unsigned int CORNER_ROUNDING_TOP_RIGHT      =   1 << 1;    // 00000010
    static const unsigned int CORNER_ROUNDING_BOTTOM_LEFT    =   1 << 2;    // 00000100
    static const unsigned int CORNER_ROUNDING_BOTTOM_RIGHT   =   1 << 3;    // 00001000

    MapTexture(int blockW, int blockH);

    int addTexture(std::string filename);

    void glVertices(double x, double y, double w, double h, int textureNumber, int cornerRounding);

    void glUnbind();

    void glBind();
};


#endif //__MapTexture_H_
