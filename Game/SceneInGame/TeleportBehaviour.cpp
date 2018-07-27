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

#include "AudioManager.h"
#include "Tile2D.h"
#include "Resources.h"
#include "Rect.h"
#include "TeleportBehaviour.h"
#include "Transform.h"
#include "GameObject.h"
#include "t2Time.h"

void TeleportBehaviour::awake() {
    sprite_ = gameObject()->getComponent<Sprite>();
}

void TeleportBehaviour::update() {

}

void TeleportBehaviour::lateUpdate() {
    transform()->setRotation(spriteOpacityPhase_ * 70.0f);

    spriteOpacityPhase_ += Tile2D::time().getDeltaTime();
    sprite_->setOpacity((1.0f + sin(spriteOpacityPhase_ * (float)M_2_PI * 8)) / 4.0f + 0.5f);

    auto pos = transform()->getPosition();

    Rect teleportRect = {
            rect_.x1 + pos.x,
            rect_.y1 + pos.y,
            rect_.x2 + pos.x,
            rect_.y2 + pos.y
    };

    if (!teleportRect.hasPointInside(player_->transform().getPosition())) {
        timer_.reset();
        enabled_ = true;
    }
    else if (timer_.getTime() > timeToGo_ && enabled_) {
        auto destination = destinationTeleport_->transform().getPosition();
        player_->transform().setPosition(destination);
        auto particles = Tile2D::resources().prefabs["fx_teleport_particles"]->instantiate();
        particles->transform().setPosition(destination);
        timer_.reset();
        destinationTeleport_->getComponent<TeleportBehaviour>()->setEnabled(false);
        if (!destinationTeleport_->isActive()) {
            destinationTeleport_->setIsActive(true);
        }

        AudioManager::getInstance()->play(Tile2D::resources().audioClips["teleport"]);
    }
}

GameObject *TeleportBehaviour::getPlayer() const {
    return player_;
}

void TeleportBehaviour::setPlayer(GameObject *player) {
    player_ = player;
}

const Rect &TeleportBehaviour::getRect() const {
    return rect_;
}

void TeleportBehaviour::setRect(const Rect &rect) {
    rect_ = rect;
}

GameObject *TeleportBehaviour::getDestinationTeleport() const {
    return destinationTeleport_;
}

void TeleportBehaviour::setDestinationTeleport(GameObject *destinationTeleport) {
    destinationTeleport_ = destinationTeleport;
}

bool TeleportBehaviour::isEnabled() const {
    return enabled_;
}

void TeleportBehaviour::setEnabled(bool enabled) {
    enabled_ = enabled;
}
