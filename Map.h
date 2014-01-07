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

#ifndef __Map_H_
#define __Map_H_

class Map {
public:
    Map(char* path);
    void setValue(int x, int y, unsigned char value);
    char getValue(int x, int y);
    short getW();
    short getH();

private:
    short w;
    short h;
    unsigned char **p_values;
    void initialize(short w, short h);

};

#endif //__Map_H_
