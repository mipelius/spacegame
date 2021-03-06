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



#include "SerializingUtils.h"
#include "AudioManager.h"
#include "Tile2D.h"
#include "t2Time.h"
#include "Health.h"
#include "Tile2DMath.h"
#include "GameObject.h"
#include "Reflector.h"
#include "Resources.h"

void Health::awake() {
    reset();
}

void Health::update() {
    heal(Tile2D::time().getDeltaTime() * autoHealingRate_);
}

void Health::lateUpdate() { }

float Health::getMaxHealth() const {
    return maxHealth_;
}

void Health::setMaxHealth(float maxHealth) {
    maxHealth_ = maxHealth;
    reset();
    clampHealth_();
}

void Health::damage(float amount, GameObject* damager) {
    if (health_ <= 0.0) { // if already dead, do nothing
        return;
    }

    health_ -= amount;
    if (health_ <= 0.0) {
        onDeath.raise(this, {damager});
    }
    else {
        if (rand() < audioPlayingProbability_ * RAND_MAX) {
            AudioManager::getInstance()->play(audioClips_, transform()->getPosition());
        }
    }

    clampHealth_();
}

void Health::reset() {
    health_ = maxHealth_;
}

void Health::heal(float amount) {
    health_ += amount;
    clampHealth_();
}

void Health::clampHealth_() {
    Mathf::clamp(health_, 0.0f, maxHealth_);
}

Health::Health() : onDeath(Event<Health, GameObjectDiedEventArgs>()) { }

float Health::getHealth() const {
    return health_;
}

float Health::getAutoHealingRate() const {
    return autoHealingRate_;
}

void Health::setAutoHealingRate(float autoHealingRate) {
    autoHealingRate_ = autoHealingRate;
}

void Health::deserialize(const json::Object &jsonObject) {
    if (jsonObject.HasKey("maxHealth")) {
        maxHealth_ = jsonObject["maxHealth"].ToFloat();
    }
    if (jsonObject.HasKey("autoHealingRate")) {
        autoHealingRate_ = jsonObject["autoHealingRate"].ToFloat();
    }
    if (jsonObject.HasKey("onDeath")) {
        auto handlerJson = jsonObject["onDeath"].ToObject();
        onDeath.deserialize(handlerJson);
    }
    if (jsonObject.HasKey("audioPlayingProbability")) {
        audioPlayingProbability_ = jsonObject["audioPlayingProbability"].ToFloat();
    }
    if (jsonObject.HasKey("audioClips")) {
        audioClips_ = utils::deserializeAudioClips(jsonObject["audioClips"].ToArray());
    }

    reset();
}

Tile2DComponent *Health::clone() {
    return new Health(*this);
}

float Health::getAudioPlayingProbability() const {
    return audioPlayingProbability_;
}

void Health::setAudioPlayingProbability(float audioPlayingProbability) {
    audioPlayingProbability_ = audioPlayingProbability;
}