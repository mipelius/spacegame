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
#include "LimitedLifetimeBehaviour.h"
#include "GameObject.h"

void LimitedLifetimeBehaviour::awake() {
    timer.reset();
}

void LimitedLifetimeBehaviour::update() {
    if (timer.getTime() > timeToLive_) {
        gameObject()->destroy();
    }
}

void LimitedLifetimeBehaviour::lateUpdate() { }

void LimitedLifetimeBehaviour::setTimeToLive(Uint32 timeToLive) {
    timeToLive_ = timeToLive;
}

void LimitedLifetimeBehaviour::deserialize(const json::Object &jsonObject) {
    if (jsonObject.HasKey("timeToLive")) {
        timeToLive_ = (Uint32)jsonObject["timeToLive"].ToInt();
    }
}

Tile2DComponent *LimitedLifetimeBehaviour::clone() {
    return new LimitedLifetimeBehaviour(*this);
}
