// This file is part of SpaceGame.
// Copyright (C) 2014 Miika Pelkonen
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

#include <iostream>
#include <cmath>
#include "PulseLight.h"
#include "Game.h"
#include "ShadowMask.h"
#include "App.h"
#include "AnimationManager.h"

void PulseLight::Animation::updateActual()  {
        double value = sin((M_PI / 2.0) + (currentFrame_ / 35.0) * (M_PI / 2.0));
        light_->pointLight_.intensity.set(value);
    };

PulseLight::Animation::Animation(PulseLight *light_) :
    AnimationBase(60, 35, false),
    light_(light_) { }

void PulseLight::Animation::stop() {
    AnimationBase::stop();
    Game::getInstance()->getShadowMask()->removeLight(&light_->pointLight_);
    App::getAnimationManager()->remove(light_->animation_);
    // delete light_;
}

PulseLight::PulseLight(Point point, double radius) :
        animation_(Animation(this)),
        pointLight_(PointLight(point, radius)) {

    initialRadius_ = radius;

    Game::getInstance()->getShadowMask()->addLight(&pointLight_);
    App::getAnimationManager()->add(animation_);
    animation_.play();
}

