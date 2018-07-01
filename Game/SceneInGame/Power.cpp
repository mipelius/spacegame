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

#include "Tile2D.h"
#include "t2Time.h"
#include "Power.h"

void Power::awake() {
    power_ = maxPower_;
}

void Power::update() {
    power_ += Tile2D::time().getDeltaTime() * reloadSpeed_;
    if (power_ > maxPower_) {
        power_ = maxPower_;
    }
}

void Power::lateUpdate() {

}

bool Power::consume(float powerConsumption) {
    if (power_ >= powerConsumption) {
        power_ -= powerConsumption;
        return true;
    }
    return false;
}

float Power::getPower() const {
    return power_;
}

float Power::getReloadSpeed() const {
    return reloadSpeed_;
}

void Power::setReloadSpeed(float reloadSpeed) {
    reloadSpeed_ = reloadSpeed;
}

float Power::getMaxPower() const {
    return maxPower_;
}

void Power::setMaxPower(float maxPower) {
    maxPower_ = maxPower;
    power_ = maxPower_;
}

void Power::deserialize(const json::Object &jsonObject) {
    if (jsonObject.HasKey("maxPower")) {
        maxPower_ = jsonObject["maxPower"].ToFloat();
    }
    if (jsonObject.HasKey("reloadSpeed")) {
        reloadSpeed_ = jsonObject["reloadSpeed"].ToFloat();
    }

    power_ = maxPower_;
}

Tile2DComponent *Power::clone() {
    return new Power(*this);
}
