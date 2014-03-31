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
#include "BlockMapping.h"
#include "Block.h"
#include "MapTexture.h"
#include "JsonFileManager.h"

BlockMapping::BlockMapping(std::string jsonFilename) : blocks_(std::vector<Block*>(255)) {
    for (int i = 0; i < 255; i++) {
        blocks_[i] = nullptr;
    }

    JsonFileManager manager;
    json::Object obj = manager.load(jsonFilename);

    json::Object commonProperties = obj["commonProperties"].ToObject();

    mapTexture_ = new MapTexture(
        commonProperties["width"].ToInt(),
        commonProperties["height"].ToInt()
    );

    json::Array blocksJson = obj["blocks"].ToArray();

    for (int i = 0; i < blocksJson.size(); i++) {
        json::Object blockJson = blocksJson[i].ToObject();
        Block* block = new Block(blockJson, mapTexture_);
        blocks_[blockJson["id"].ToInt()] = block;
    }
}

Block *BlockMapping::getBlock(unsigned char id) {
    return blocks_[id];
}

MapTexture *BlockMapping::getMapTexture() {
    return mapTexture_;
}