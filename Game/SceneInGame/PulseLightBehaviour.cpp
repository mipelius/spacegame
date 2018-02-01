// This file is part of SpaceGame.
// Copyright (C) 2018 Miika Pelkonen
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

#include "PulseLightBehaviour.h"

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
