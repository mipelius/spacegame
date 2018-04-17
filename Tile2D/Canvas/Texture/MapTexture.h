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


#ifndef __MapTexture_H_
#define __MapTexture_H_

#include "precompile.h"

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

    int addTexture(std::string filename, float opacity);

    void glVertices(float x, float y, float w, float h, int textureNumber, int cornerRounding);

    void glUnbind();

    void glBind();
};


#endif //__MapTexture_H_
