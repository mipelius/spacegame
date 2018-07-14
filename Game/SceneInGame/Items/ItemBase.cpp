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
#include "AudioClip.h"
#include "ItemBase.h"
#include "GameObject.h"
#include "Resources.h"

bool ItemBase::use(GameObject *user) {
    if (timer_.getTime() < reloadDelay_) {
        return false;
    }
    timer_.reset();

    auto power = user->getComponent<Power>();

    if (!isActivated_) {
        return false;
    }
    if (power == nullptr) {
        return use_(user);
    }
    if (power->getPower() < powerConsumption_) {
        return false;
    }
    if (use_(user)) {
        power->consume(powerConsumption_);
        return true;
    }
    return false;
}



float ItemBase::getPowerConsumption() const {
    return powerConsumption_;
}

void ItemBase::setPowerConsumption(float powerConsumption) {
    powerConsumption_ = powerConsumption;
}

bool ItemBase::isActivated() const {
    return isActivated_;
}

void ItemBase::setIsActivated(bool isActivated) {
    isActivated_ = isActivated;
}

int ItemBase::getCount() const {
    return count_;
}

void ItemBase::setCount(int count) {
    count_ = count;
}

int ItemBase::getReloadDelay() const {
    return reloadDelay_;
}

void ItemBase::setReloadDelay(int reloadDelay) {
    reloadDelay_ = reloadDelay;
}


bool ItemBase::use_(GameObject *user) {
    if (count_ > 0 || count_ == COUNT_INFINITY) {
        if (useActual(user)) {
            if (count_ > 0) {
                count_--;
            }

            AudioManager::getInstance()->play(audioClip_);

            return true;
        }
    }

    return false;
}

void ItemBase::deserialize(const json::Object &jsonObject) {
    if (jsonObject.HasKey("isActivated")) {
        isActivated_ = jsonObject["isActivated"].ToBool();
    }
    if (jsonObject.HasKey("powerConsumption")) {
        powerConsumption_ = jsonObject["powerConsumption"].ToFloat();
    }
    if (jsonObject.HasKey("count")) {
        auto countJson = jsonObject["count"];
        if (countJson.GetType() == json::IntVal) {
            count_ = countJson.ToInt();
        }
        else if (countJson.GetType() == json::StringVal) {
            if (countJson.ToString() != "infinity") {
                throw std::runtime_error("ItemBase: count has invalid value. It should be int or string \"infinity\"");
            }
            count_ = COUNT_INFINITY;
        }
    }
    if (jsonObject.HasKey("reloadDelay")) {
        reloadDelay_ = jsonObject["reloadDelay"].ToInt();
    }
    if (jsonObject.HasKey("audioClip")) {
        auto audioClipName = jsonObject["audioClip"].ToString();
        audioClip_ = Tile2D::resources().audioClips[audioClipName];
    }
}