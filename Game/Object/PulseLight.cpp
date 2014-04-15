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
#include <math.h>
#include "PulseLight.h"
#include "Point.h"
#include "AnimationBase.h"
#include "PointLight.h"
#include "Game.h"
#include "ShadowMask.h"
#include "App.h"
#include "AnimationManager.h"

class PulseLight::Animation : public AnimationBase {

private:
    PulseLight* light_;

protected:
    virtual void updateActual() {
        double value = sin((M_PI / 2.0) + (currentFrame_ / 35.0) * (M_PI / 2.0));
        light_->pointLight_->intensity->set(value);
    };

public:
    Animation(PulseLight *light_) :
    AnimationBase(60, 35, false),
    light_(light_) { }

    void stop() {
        AnimationBase::stop();
        Game::getInstance()->getShadowMask()->removeLight(light_->pointLight_);
        light_->animation_->die();
    }

    void onDie() {
        delete light_;
    }
};

PulseLight::PulseLight(Point point, double radius) {
    initialRadius_ = radius;

    pointLight_ = new PointLight(point, radius);
    Game::getInstance()->getShadowMask()->addLight(pointLight_);

    animation_ = new Animation(this);
    App::getInstance()->getAnimationManager()->add(animation_);
    animation_->play();
}

PulseLight::~PulseLight() {
    delete pointLight_;
}
