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


#ifndef __JsonFileManager_H_
#define __JsonFileManager_H_

#include "json.h"

class JsonFileManager {

public:
    static json::Object load(std::string filename);
    static json::Object load(
            const std::string&  jsonTemplateFilePath,
            const json::Object& templateReplacementJsonObject
    );
    static void save(json::Object object, std::string filename);

    JsonFileManager() = delete;
    ~JsonFileManager() = delete;

    JsonFileManager(const JsonFileManager& other) = delete;
    JsonFileManager& operator=(const JsonFileManager& other) = delete;

    JsonFileManager(JsonFileManager&& other) = delete;
    JsonFileManager& operator=(JsonFileManager&& ) = delete;


private:
    static std::string stringify_(const json::Value& jsonValue);


};


#endif //__JsonFileManager_H_
