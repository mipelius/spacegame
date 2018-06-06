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

//    static_assert(
//            std::is_constructible<T, const std::string&>::value,
//            "T doesn't have constructor (const std::string&)"
//    );

    ResourceContainer() = default;

    void init(const json::Value& resourcesJson) {
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
