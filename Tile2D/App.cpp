//
// Created by Miika Pelkonen on 2/3/14.
// Copyright (c) 2014 ___MIIKAPELKONEN___. All rights reserved.
//

#include "App.h"
#include "SamplePlayer.h"
#include "MusicPlayer.h"
#include "Window.h"

App* App::instance_ = nullptr;

App::App() {
    this->musicPlayer_ = MusicPlayer::getInstance();
    this->samplePlayer_ = new SamplePlayer();
    this->window_ = Window::getInstance();
}

App::~App() {
    delete musicPlayer_;
    delete samplePlayer_;
    delete window_;
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
