// MIT License
//
// This file is part of SpaceGame.
// Copyright (c) 2014-2018 Miika Pelkonen
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


#include <iostream>
#include "TileSet.h"
#include "MapTexture.h"
#include "JsonFileManager.h"
#include "Tile.h"

TileSet::TileSet(std::string jsonFilename) :
        blocks_(std::vector<Tile*>(256))
{
    emptyBlock_ = new Tile("empty block", 0.0, 1.0, 1.0, {-1, 0, 0, nullptr});

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
