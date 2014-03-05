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

#include "App.h"
#include "SamplePlayer.h"
#include "MusicPlayer.h"
#include "Window.h"
#include "AnimationManager.h"

App* App::instance_ = nullptr;

App::App() {
    this->musicPlayer_ = MusicPlayer::getInstance();
    this->samplePlayer_ = new SamplePlayer();
    this->window_ = Window::getInstance();
    this->animationManager_ = new AnimationManager();
}

App::~App() {
    delete musicPlayer_;
    delete samplePlayer_;
    delete window_;
    delete animationManager_;
}

App *App::getInstance() {
    if (instance_) initialize();
    return instance_;
}

void App::initialize() {
    if (!instance_) instance_ = new App();
}

Window *App::getWindow() {
    return window_;
}

MusicPlayer *App::getMusicPlayer() {
    return musicPlayer_;
}

SamplePlayer *App::getSamplePlayer() {
    return samplePlayer_;
}

AnimationManager *App::getAnimationManager() {
    return animationManager_;
}
