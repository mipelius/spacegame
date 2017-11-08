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
#include "MapTexture.h"
#include "JsonFileManager.h"
#include "Block.h"

BlockMapping::BlockMapping(std::string jsonFilename) :
        blocks_(std::vector<Block*>(255))
{
    emptyBlock_ = new Block("empty block", 0.0, 1.0, 1.0, nullptr, -1);

    for (int i = 0; i < 256; i++) {
        blocks_[i] = nullptr;
    }

    JsonFileManager manager;
    json::Object obj = manager.load(std::move(jsonFilename));

    json::Object commonProperties = obj["commonProperties"].ToObject();

    mapTexture_ = new MapTexture(
        commonProperties["width"].ToInt(),
        commonProperties["height"].ToInt()
    );

    json::Array blocksJson = obj["blocks"].ToArray();

    for (const auto &jsonObj : blocksJson) {
        auto blockJson = jsonObj.ToObject();
        blocks_[blockJson["id"].ToInt()] = new Block(blockJson, mapTexture_);
    }
}

Block* BlockMapping::getBlock(unsigned char id) {
    return blocks_[id];
}

MapTexture * BlockMapping::getMapTexture() {
    return mapTexture_;
}

BlockMapping::~BlockMapping() {
    delete emptyBlock_;
    delete mapTexture_;
    for (auto block : blocks_) {
        if (block != nullptr) delete block;
    }
}

Block* BlockMapping::getEmptyBlock() {
    return emptyBlock_;
}