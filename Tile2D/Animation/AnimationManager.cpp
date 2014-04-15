// This file is part of SpaceGame.
// Copyright (C) 2014  Miika Pelkonen
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

#include "AnimationManager.h"
#include "IAnimation.h"

AnimationManager::AnimationManager() {

}

AnimationManager::~AnimationManager() {

}

void AnimationManager::add(IAnimation *animation) {
    animations_.push_back(animation);
}



void AnimationManager::update(double seconds) {
    std::list<IAnimation*> animationsToRemove;

    for (std::list<IAnimation*>::iterator i = animations_.begin(); i != animations_.end(); i++) {
        if ((*i)->isDead()) {
            animationsToRemove.push_back((*i));
        }
        else {
            (*i)->update(seconds);
        }
    }

    for (std::list<IAnimation*>::iterator i = animationsToRemove.begin(); i != animationsToRemove.end(); i++) {
        remove((*i));
        delete (*i);
    }
}

void AnimationManager::remove(IAnimation *animation) {
    animations_.remove(animation);
}
