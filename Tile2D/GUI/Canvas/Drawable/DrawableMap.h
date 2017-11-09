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

#ifndef __DrawableMap_H_
#define __DrawableMap_H_

class WorldMap;
class MapTexture;

#include "IDrawable.h"

class DrawableMap : public IDrawable {
public:
    DrawableMap();

    void setMap(WorldMap * map);
    void setMapTexture(MapTexture* mapTexture);

    void draw(const Canvas &canvas) override;

private:
    MapTexture* mapTexture_;
    WorldMap * map_;

    void drawMap(const Canvas& canvas);
    void drawSmallMap(const Canvas& canvas);

};

#endif //__DrawableMap_H_
