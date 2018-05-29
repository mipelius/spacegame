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


#include "PulseLightBehaviour.h"
#include "GameObject.h"

void PulseLightBehaviour::awake() {
    LimitedLifetimeBehaviour::awake();
    light = gameObject()->getComponent<PointLight>();
    radiusOnAwake_ = light->getRadius();
    intensityOnAwake_ = light->getIntensity();
}

void PulseLightBehaviour::update() {
    LimitedLifetimeBehaviour::update();
    if (timer.getTime() < timeToStartDiminish_) {
        return;
    }
    float scale = (timer.getTime() - timeToStartDiminish_) / (float)(timer.getInterval() - timeToStartDiminish_);
    light->setIntensity((1.0f - scale * intensityDiminishSpeed_) * intensityOnAwake_);
    light->setRadius((1.0f - scale * radiusDiminishSpeed_) * radiusOnAwake_);
}

void PulseLightBehaviour::lateUpdate() { }

float PulseLightBehaviour::getRadiusDiminishSpeed() const {
    return radiusDiminishSpeed_;
}

void PulseLightBehaviour::setRadiusDiminishSpeed(float radiusDiminishSpeed) {
    radiusDiminishSpeed_ = radiusDiminishSpeed;
}

float PulseLightBehaviour::getIntensityDiminishSpeed() const {
    return intensityDiminishSpeed_;
}

void PulseLightBehaviour::setIntensityDiminishSpeed(float intensityDiminishSpeed) {
    intensityDiminishSpeed_ = intensityDiminishSpeed;
}

Uint32 PulseLightBehaviour::getTimeToStartDiminish() const {
    return timeToStartDiminish_;
}

void PulseLightBehaviour::setTimeToStartDiminish(Uint32 timeToStartDiminish) {
    timeToStartDiminish_ = timeToStartDiminish;
}
