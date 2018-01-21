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

#ifndef __Block_H_
#define __Block_H_

class MapTexture;

#include <string>
#include "json.h"

class Tile {

public:
    Tile(std::string name, float density, float translucency, float opacity, MapTexture* mapTexture, int mapTextureId);
    Tile(json::Object object, MapTexture* mapTexture);

    int getMapTextureId();

    // getters

    const std::string &getName() const;
    float getDensity() const;
    float getTranslucency() const;
    float getOpacity() const;

private:
    std::string name_;
    float density_;
    float translucency_;
    float opacity_;

    MapTexture* mapTexture_;
    int mapTextureId_;

    void initialize_(std::string name, float density, float translucency, float opacity, MapTexture *mapTexture,
                     int mapTextureId);

};


#endif //__Block_H_
