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


class MapTexture {
private:
    GLuint texture;
    int blockW;
    int blockH;
    int count;

    void createGlTexture(SDL_Surface*);

public:
    MapTexture(int blockW, int blockH, int count, std::string fileNames[]);

    void renderBlock(int x, int y, int w, int h, int textureNumber);

    void glUnbind();

    void glBind();
};


#endif //__MapTexture_H_
