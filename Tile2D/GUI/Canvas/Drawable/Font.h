// This file is part of SpaceGame.
// Copyright (C) 2015 Miika Pelkonen
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

#ifndef __Font_H_
#define __Font_H_

class Texture;

#include <string>
#include <map>

class Font {

friend class Text;

public:
    explicit Font(std::string filename);
    ~Font();

private:
    class Letter {
    public:
        float x, y, w, h;

        Letter(float x, float y, float w, float h) {
            this->x = x;
            this->y = y;
            this->w = w;
            this->h = h;
        }
    };

    Texture* fontTexture_;

    Letter* getLetter(unsigned char ch);

    std::map<unsigned char, Letter*> mappings;
};


#endif //__Font_H_
