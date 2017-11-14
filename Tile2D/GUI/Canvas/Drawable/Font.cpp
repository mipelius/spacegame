// This file is part of SpaceGame.
// Copyright (C) 2015 Miika Pelkonen
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
#include "Font.h"
#include "Texture.h"
#include "JsonFileManager.h"

Font::Font(std::string filename) {
    json::Object object = JsonFileManager::load(std::move(filename));

    std::string fontTextureFilename = object["imgFile"];

    fontTexture_ = new Texture(fontTextureFilename);

    json::Array mappingsArray = object["mappings"];

    for (int i = 0; i < mappingsArray.size(); i++) {
        json::Object mapping = mappingsArray[i].ToObject();

        auto letter = new Letter(
                mapping["x"].ToFloat(),
                mapping["y"].ToFloat(),
                mapping["w"].ToFloat(),
                mapping["h"].ToFloat()
        );

        std::string letters = mapping["letters"].ToString();

        for (int j=0; j < letters.length(); j++) {
            auto ch = (unsigned char)letters.data()[j];
            std::pair<unsigned char, Letter*> pair = std::pair<unsigned char, Letter*>(ch, letter);
            mappings.insert(pair);
        }
    }

}

Font::~Font() {
    for (auto& pair : mappings) {
        Letter* letter = pair.second;
        delete letter;
    }
}


Font::Letter *Font::getLetter(unsigned char ch) {
    return mappings[ch];
}
