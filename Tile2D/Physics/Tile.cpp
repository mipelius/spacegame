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

#include "Tile.h"
#include "MapTexture.h"

Tile::Tile(std::string name, float density, float translucency, float opacity, MapTexture *mapTexture, int mapTextureId)
{
    initialize_(std::move(name), density, translucency, opacity, mapTexture, mapTextureId);
}

Tile::Tile(json::Object object, MapTexture* mapTexture)
{
    MapTexture* mapTextureToUse = nullptr;
    int mapTextureId = -1;

    json::Value nullValue;

    if (object["texture"] != nullValue) {
        mapTextureToUse = mapTexture;

        mapTextureId = mapTexture->addTexture(
                object["texture"].ToString()
        );
    }

    initialize_(
            object["name"].ToString(),
            object["density"].ToFloat(),
            object["translucency"].ToFloat(),
            object["opacity"].ToFloat(),
            mapTextureToUse,
            mapTextureId
    );
}

void Tile::initialize_(std::string name, float density, float translucency, float opacity, MapTexture *mapTexture,
                        int mapTextureId) {
    name_ = std::move(name);
    density_ = density;
    translucency_ = translucency;
    opacity_ = opacity;
    mapTexture_ = mapTexture;
    mapTextureId_ = mapTextureId;
}

// getters

int Tile::getMapTextureId() {
    return mapTextureId_;
}

const std::string &Tile::getName() const {
    return name_;
}

float Tile::getDensity() const {
    return density_;
}

float Tile::getTranslucency() const {
    return translucency_;
}

float Tile::getOpacity() const {
    return opacity_;
}
