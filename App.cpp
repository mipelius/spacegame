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
#include <SDL2/SDL.h>
#include "SamplePlayer.h"
#include "MusicPlayer.h"
#include "Window.h"
#include "AnimationManager.h"

App* App::instance_ = nullptr;

App::App() {
    SDL_Init(SDL_INIT_EVERYTHING);

    // --- WINDOW PROPERTIES ---

    int x = 0;
    int y = 0;
    int w = 1280;
    int h = 800;

    bool enableFullScreen = false;

    window_ = Window::getInstance();
    window_->initialize(x, y, w, h, enableFullScreen);

    musicPlayer_ = MusicPlayer::getInstance();
    samplePlayer_ = new SamplePlayer();
    animationManager_ = new AnimationManager();
    resources_ = new Resources();
}

App::~App() {
    delete musicPlayer_;
    delete samplePlayer_;
    delete window_;
    delete animationManager_;
    delete resources_;
}

void App::initialize() {
    if (!instance_) {
        instance_ = new App();
    }
}

Window *App::getWindow() {
    return instance_->window_;
}

MusicPlayer *App::getMusicPlayer() {
    return instance_->musicPlayer_;
}

SamplePlayer *App::getSamplePlayer() {
    return instance_->samplePlayer_;
}

AnimationManager *App::getAnimationManager() {
    return instance_->animationManager_;
}

Resources *App::getResources() {
    return instance_->resources_;
}

void App::free() {
    delete instance_;
}
