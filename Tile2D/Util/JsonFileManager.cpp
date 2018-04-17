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


#include "precompile.h"
#include "JsonFileManager.h"
#include "Tile2D.h"
#include <algorithm>

json::Object JsonFileManager::load(std::string filename) {
    json::Object obj;

    filename = Tile2D::getResourcePath() + filename;

#ifdef _WIN32
	std::replace(filename.begin(), filename.end(), '/', '\\');
#endif /* _WIN32 */
#ifdef __APPLE__
    std::replace(filename.begin(), filename.end(), '\\', '/');
#endif /* __APPLE__ */

    std::ifstream file(filename);
	
    if (file.is_open()) {
        std::stringstream buffer;
        buffer << file.rdbuf();

        try {
            obj = json::Deserialize(buffer.str());
        }
        catch(std::exception& ex) {
            file.close();
            std::string error = "Broken json file : \" + filename";
            throw std::runtime_error(error);
        }

        file.close();
    }
    else {
        std::string error = "file not found: ";
        error.append(filename);
        throw std::runtime_error(error);
    }

    return obj;
}

void JsonFileManager::save(json::Object object, std::string filename) {
    std::ofstream file;
    file.open (filename);
    file << json::Serialize(object);
    file.close();
}
