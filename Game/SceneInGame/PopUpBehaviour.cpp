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

#include "PopUpBehaviour.h"
#include "DrawableBase.h"
#include "GameObject.h"

void PopUpBehaviour::deserialize(const json::Object &jsonObject) {
    LimitedLifetimeBehaviour::deserialize(jsonObject);

    if (jsonObject.HasKey("startScale")) {
        startScale_.deserialize(jsonObject["startScale"]);
    }
    else {
        startScale_ = transform()->getScale();
    }

    if (jsonObject.HasKey("endScale")) {
        endScale_.deserialize(jsonObject["endScale"]);
    }
}

void PopUpBehaviour::awake() {
    LimitedLifetimeBehaviour::awake();

    drawable_ = gameObject()->getComponent<DrawableBase>();
}

void PopUpBehaviour::update() {
    LimitedLifetimeBehaviour::update();
    auto phase = timer.getTime() / (float)timeToLive_;
    auto scale = startScale_ * (1.0f - phase) + endScale_ * phase;
    transform()->setScale(scale);

    drawable_->setOpacity(sin(phase * (float)M_PI));
}

void PopUpBehaviour::lateUpdate() {
    LimitedLifetimeBehaviour::lateUpdate();
}

Tile2DComponent *PopUpBehaviour::clone() {
    return new PopUpBehaviour(*this);
}

const Vecf &PopUpBehaviour::getStartScale() const {
    return startScale_;
}

void PopUpBehaviour::setStartScale(const Vecf &startScale) {
    startScale_ = startScale;
}

const Vecf &PopUpBehaviour::getEndScale() const {
    return endScale_;
}

void PopUpBehaviour::setEndScale(const Vecf &endScale) {
    endScale_ = endScale;
}
