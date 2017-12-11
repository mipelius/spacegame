// This file is part of SpaceGame.
// Copyright (C) 2017 Miika Pelkonen
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

#ifndef __ResourceContainer_H
#define __ResourceContainer_H

#include <string>
#include <vector>
#include <map>
#include "json.h"

template <class T>
class ResourceContainer {
    friend class Resources;

private:
    std::map<std::string, T*> resourceMap_;

    static_assert(
            std::is_constructible<T, const std::string&>::value,
            "T doesn't have constructor (const std::string&)"
    );

    ResourceContainer() = default;

    void init(json::Value resourcesJson) {
        auto resArray = resourcesJson.ToArray();

        for (const auto &resValue : resArray) {
            std::string resPath = resValue.ToString();

            auto res = new T(resPath);

            unsigned long first = resPath.find_last_of('/') + 1;
            unsigned long n = resPath.find_last_of('.') - first;

            std::string shortName = resPath.substr(first, n);

            resourceMap_[shortName] = res;
        }
    }

    ~ResourceContainer() {
        for (auto &res : resourceMap_) {
            delete res.second;
        }
    }

public:
    T* operator[](std::string str) {
        auto result = resourceMap_[str];
        if (result == nullptr) {
            throw std::runtime_error("no such resource" + str);
        }
        return result;
    }
};

#endif //__ResourceContainer_H
