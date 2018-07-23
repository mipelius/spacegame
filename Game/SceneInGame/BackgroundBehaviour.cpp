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



#include "MusicManager.h"
#include "BackgroundBehaviour.h"
#include "Tile2D.h"
#include "Tile2DMath.h"
#include "Camera.h"
#include "GameObject.h"
#include "AudioSource.h"
#include "t2Time.h"
#include "AudioClip.h"
#include "Resources.h"

void BackgroundBehaviour::awake() {
    bg_ = gameObject()->getComponent<Background>();
}

void BackgroundBehaviour::update() {

}

void BackgroundBehaviour::lateUpdate() {
    Camera* camera = Tile2D::canvas().getCamera();

    float opacity = bg_->getOpacity();

    if (area_.hasPointInside(camera->getPosition())) {
        MusicManager::getInstance()->play(music_);
        opacity += fadeInOutSpeed_ * Tile2D::time().getDeltaTime();
    } else {
        opacity -= fadeInOutSpeed_ * Tile2D::time().getDeltaTime();
    }

    Mathf::clamp(opacity, 0.0f, 1.0f);

    bg_->setOpacity(opacity);
}

void BackgroundBehaviour::setArea(const Rect &area) {
    BackgroundBehaviour::area_ = area;
}

void BackgroundBehaviour::deserialize(const json::Object &jsonObject) {
    if (jsonObject.HasKey("area")) {
        area_.deserialize(jsonObject["area"].ToObject());
    }
    if (jsonObject.HasKey("fadeOutSpeed")) {
        fadeInOutSpeed_ = jsonObject["fadeOutSpeed"].ToFloat();
    }
    if (jsonObject.HasKey("music")) {
        auto musicName = jsonObject["music"].ToString();
        music_ = Tile2D::resources().audioClips[musicName];
    }
}

Tile2DComponent *BackgroundBehaviour::clone() {
    return new BackgroundBehaviour(*this);
}
