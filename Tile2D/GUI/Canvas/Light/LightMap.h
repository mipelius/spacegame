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

#ifndef __LightMap_H_
#define __LightMap_H_

class Map;

class LightMap {

    friend class ShadowMap;

public:
    LightMap(int w, int h);
    ~LightMap();

    void clear();
    void applyLight(int x, int y, Map* map, int offsetX, int offsetY);
    void applyLightCenter(Map* map, int offsetX, int offsetY);

private:
    int w_;
    int h_;
    double* data_;

    void applyLightInternal(int x, int y, Map* map, int offsetX, int offsetY, double lastLight);
};

#endif //__LightMap_H_
