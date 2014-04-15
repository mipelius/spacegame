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
#include "SpriteContainer.h"
#include "Sprite.h"
#include "PointLight.h"
#include "ShadowMask.h"

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

Texture* Explosion::explosionTexture_ = nullptr;

Explosion::Explosion(Point point, double radius) {
    if (!explosionTexture_) {
        explosionTexture_ = new Texture("images/anim_explosion.png");
    }

    explosionAnimation_ = new AnimatedTexture(40, 8, false, explosionTexture_);
    explosionAnimation_->play();
    App::getInstance()->getAnimationManager()->add(explosionAnimation_);

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

    spriteContainer_ = new SpriteContainer();
    spriteContainer_->addSprite(explosionSprite);
    spriteContainer_->location->set(point);

    light_ = new PointLight(point, radius * 3);
    Game::getInstance()->getShadowMask()->addLight(light_);

    Game::getInstance()->getCanvas()->addDrawable(spriteContainer_);
}

Explosion::~Explosion() {
    Game::getInstance()->getCanvas()->removeDrawable(spriteContainer_);
    explosionAnimation_->die();
}
