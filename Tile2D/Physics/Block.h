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
#include "ReadableProperty.h"
#include "json.h"

class Block {

private:
    std::string name_;
    double density_;
    double translucency_;
    double opacity_;

    MapTexture* mapTexture_;
    int mapTextureId_;

    void initialize(std::string name, double density, double translucency, double opacity, MapTexture* mapTexture, int mapTextureId);

public:
    Block(std::string name, double density, double translucency, double opacity, MapTexture* mapTexture, int mapTextureId);
    Block(json::Object object, MapTexture* mapTexture);
    ~Block();

    ReadableProperty<std::string>* const name;
    ReadableProperty<double>* const density;
    ReadableProperty<double>* const translucency;
    ReadableProperty<double>* const opacity;

    int getMapTextureId();

};


#endif //__Block_H_
