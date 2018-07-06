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
#include "JsonFileManager.h"
#include "SceneManager.h"
#include "Tile2D.h"
#include "Reflector.h"

SceneManager::SceneManager() {
    sceneToLoad_ = -1;
    currentScene_ = -1;
}

SceneManager::~SceneManager() {
    if (currentScene_ != -1) {
        scenes_[currentScene_]->destroy();
    }
    for (auto& scene : scenes_) {
        delete scene;
    }
}

void SceneManager::loadScene(unsigned scene) {
    sceneToLoad_ = scene;
}


void SceneManager::init(const std::string &scenesFile) {
    auto json = JsonFileManager::load(scenesFile);
    auto scenesJson = json["scenes"].ToArray();

    for (const json::Value& sceneJsonValue : scenesJson) {
        auto sceneFilePath = sceneJsonValue.ToString();
        auto sceneJson = JsonFileManager::load(sceneFilePath);
        auto scene = Tile2D::reflector().instantiate<IScene>(sceneJson);
        scenes_.push_back(scene);
    }

    loadScene(0);
}

void SceneManager::update_() {
    if (sceneToLoad_ == -1) {
        return;
    }

    Tile2D::instance_().cleanUp_();

    if (currentScene_ != -1) {
        scenes_[currentScene_]->destroy();
    }
    currentScene_ = sceneToLoad_;
    int tmp = sceneToLoad_;
    sceneToLoad_ = -1;
    scenes_[tmp]->init();
}
