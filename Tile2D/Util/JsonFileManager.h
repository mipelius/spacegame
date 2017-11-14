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

#ifndef __JsonFileManager_H_
#define __JsonFileManager_H_

#include "json.h"

class JsonFileManager {

public:
    static json::Object load(std::string filename);
    static void save(json::Object object, std::string filename);

    JsonFileManager() = delete;
    ~JsonFileManager() = delete;

    JsonFileManager(const JsonFileManager& other) = delete;
    JsonFileManager& operator=(const JsonFileManager& other) = delete;

    JsonFileManager(JsonFileManager&& other) = delete;
    JsonFileManager& operator=(JsonFileManager&& ) = delete;
};


#endif //__JsonFileManager_H_
