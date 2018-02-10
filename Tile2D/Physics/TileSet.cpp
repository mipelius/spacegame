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

#include <iostream>
#include "TileSet.h"
#include "MapTexture.h"
#include "JsonFileManager.h"
#include "Tile.h"

TileSet::TileSet(std::string jsonFilename) :
        blocks_(std::vector<Tile*>(256))
{
    emptyBlock_ = new Tile("empty block", 0.0, 1.0, 1.0, nullptr, -1);

    for (int i = 0; i < 256; i++) {
        blocks_[i] = nullptr;
    }

    json::Object obj = JsonFileManager::load(std::move(jsonFilename));

    json::Object commonProperties = obj["commonProperties"].ToObject();

    tileW_ = commonProperties["width"].ToInt();
    tileH_ = commonProperties["height"].ToInt();

    mapTexture_ = new MapTexture(
        commonProperties["width"].ToInt(),
        commonProperties["height"].ToInt()
    );

    json::Array blocksJson = obj["blocks"].ToArray();

    for (const auto &jsonObj : blocksJson) {
        auto blockJson = jsonObj.ToObject();
        blocks_[blockJson["id"].ToInt()] = new Tile(blockJson, mapTexture_);
    }
}

Tile* TileSet::getTile(unsigned char id) {
    return blocks_[id];
}

MapTexture * TileSet::getMapTexture() {
    return mapTexture_;
}

TileSet::~TileSet() {
    delete emptyBlock_;
    delete mapTexture_;
    for (auto block : blocks_) {
        if (block != nullptr) delete block;
    }
}

Tile* TileSet::getEmptyBlock() {
    return emptyBlock_;
}

int TileSet::getTileW() {
    return tileW_;
}

int TileSet::getTileH() {
    return tileH_;
}
