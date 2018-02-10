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

#include "precompile.h"
#include "JsonFileManager.h"
#include <algorithm>

json::Object JsonFileManager::load(std::string filename) {
    json::Object obj;

	std::replace(filename.begin(), filename.end(), '/', '\\');
	
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
