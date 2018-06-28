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

#include "JsonFileManager.h"
#include "Animation.h"

int Animation::getFrames() const {
    return frames_;
}

Animation::Animation(std::string filepath) {
    filepath_ = filepath;
}

Animation::~Animation() {
    delete texture_;
}

Rect Animation::getTexCoords(int frame) const {
    int framesPerRow = texture_->getW() / spriteWidth_;
    int framesPerColumn = texture_->getH() / spriteHeight_;

    float frameTexWidth = 1.0f / framesPerRow;
    float frameTexHeight = 1.0f / framesPerColumn;

    float x1 = (frame % framesPerRow) * frameTexWidth;
    float y1 = (frame / framesPerRow) * frameTexHeight;
    float x2 = x1 + frameTexWidth;
    float y2 = y1 + frameTexHeight;

    // margins

    x1 += 0.001;
    y1 -= 0.001;
    x2 += 0.001;
    y2 -= 0.001;

    return {x1, y1, x2, y2};
}

const Texture *Animation::getTexture() const {
    return texture_;
}

void Animation::reload() {
    delete texture_;

    auto jsonObj = JsonFileManager::load(filepath_);

    spriteWidth_ = jsonObj["spriteWidth"].ToInt();
    spriteHeight_ = jsonObj["spriteHeight"].ToInt();
    frames_ = jsonObj["frames"].ToInt();

    auto texturePath = jsonObj["texture"].ToString();
    texture_ = new Texture(texturePath);
    texture_->reload();
}
