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
#include <set>
#include "Font.h"
#include "Texture.h"
#include "JsonFileManager.h"

Font::Font(std::string filename) {
    json::Object object = JsonFileManager::load(std::move(filename));

    std::string fontTextureFilename = object["imgFile"];

    fontTexture_ = new Texture(fontTextureFilename, GL_NEAREST, GL_NEAREST);

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
    std::set<Letter*> lettersToDelete;

    for (auto& pair : mappings) {
        if (lettersToDelete.find(pair.second) == lettersToDelete.end()) {
            lettersToDelete.insert(pair.second);
        }
    }

    for (auto& letter : lettersToDelete) {
        delete letter;
    }
}


Font::Letter *Font::getLetter(const char& ch) {
    return mappings[ch];
}
