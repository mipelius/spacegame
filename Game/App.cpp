//
// Created by Miika Pelkonen on 2/3/14.
// Copyright (c) 2014 ___MIIKAPELKONEN___. All rights reserved.
//

#include "App.h"
#include "SamplePlayer.h"
#include "MusicPlayer.h"

App* App::_instance = nullptr;

App::App() {
    this->musicPlayer = new MusicPlayer();
    this->samplePlayer = new SamplePlayer();
}

App::~App() {
    delete this->musicPlayer;
    delete this->samplePlayer;
}

MusicPlayer *App::getMusicPlayer() {
    return _instance->musicPlayer;
}

SamplePlayer *App::getSamplePlayer() {
    return _instance->samplePlayer;
}

App *App::instance() {
    if (_instance) initialize();
    return _instance;
}

void App::initialize() {
    if (!_instance) _instance = new App();
}
