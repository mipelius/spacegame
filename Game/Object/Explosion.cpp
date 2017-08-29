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

#include "Explosion.h"
#include "Texture.h"
#include "AnimatedTexture.h"
#include "Game.h"
#include "Canvas.h"
#include "App.h"
#include "AnimationManager.h"
#include "DrawableGroup.h"
#include "Sprite.h"
#include "PointLight.h"
#include "ShadowMask.h"
#include "PulseLight.h"
#include "Sample.h"
#include "SamplePlayer.h"

class Explosion::AnimatedTexture_Stopped : public IEventHandler<AnimatedTexture, EventArgs> {

private:
    Explosion* explosion_;

public:
    AnimatedTexture_Stopped(Explosion* explosion) {
        explosion_ = explosion;
    }

    virtual void handle(AnimatedTexture *owner, EventArgs args) {
        delete explosion_;
    }
};

int Explosion::sampleChannel_ = 0;

Explosion::Explosion(Point point, double radius) {
    sampleChannel_ = 0;

    explosionAnimation_ = new AnimatedTexture(32, 8, false, App::getResources()->textures->animExplosion);
    App::getAnimationManager()->add(explosionAnimation_);
    explosionAnimation_->play();

    AnimatedTexture_Stopped* handler = new AnimatedTexture_Stopped(this);
    explosionAnimation_->stopped->add(handler);

    Sprite* explosionSprite = new Sprite(
            explosionAnimation_,
            Rect(
                    -radius,
                    -radius,
                    radius,
                    radius
            )
    );

    App::getSamplePlayer()->play(App::getResources()->samples->explosion, sampleChannel_, &point);
    sampleChannel_++;
    if (sampleChannel_ > 7) sampleChannel_ = 0;

    spriteContainer_ = new DrawableGroup();
    spriteContainer_->addDrawable(explosionSprite);
    spriteContainer_->location.set(point);

    PulseLight* pulseLight = new PulseLight(point, radius * 2.5);

    Game::getInstance()->getExternalCanvas()->addDrawable(spriteContainer_);
}

Explosion::~Explosion() {
    Game::getInstance()->getExternalCanvas()->removeDrawable(spriteContainer_);
    explosionAnimation_->die();
}
