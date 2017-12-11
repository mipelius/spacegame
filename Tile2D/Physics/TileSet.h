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

#ifndef __BlockMapping_H_
#define __BlockMapping_H_

#include <vector>
#include <string>

class MapTexture;
class Tile;

class TileSet {

public:
    explicit TileSet(std::string jsonFilename);
    ~TileSet();

    Tile* getTile(unsigned char id);
    MapTexture * getMapTexture();

    Tile* getEmptyBlock();

private:
    std::vector<Tile*> blocks_;
    MapTexture* mapTexture_;
    Tile* emptyBlock_;
};

#endif //__BlockMapping_H_
