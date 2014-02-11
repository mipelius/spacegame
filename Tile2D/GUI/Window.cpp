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

#include "precompile.h"
#include "Window.h"
#include "GuiComponentBase.h"

Window* Window::instance_ = nullptr;

Window::Window() { }

Window::~Window() {
    if (window_) SDL_DestroyWindow(window_);
}

Window* Window::getInstance() {
    if (!instance_) instance_ = new Window();
    return instance_;
}

void Window::initialize(int x, int y, int w, int h, bool enableFullScreen) {
    x_ = x;
    y_ = y;
    w_ = w;
    h_ = h;

    // Window initialization
    SDL_Init(SDL_INIT_VIDEO);

    window_ = SDL_CreateWindow(
            "",
            x,
            y,
            w,
            h,
            SDL_WINDOW_OPENGL | (enableFullScreen ? SDL_WINDOW_FULLSCREEN : 0)
    );

    if (!window_) {
        std::fprintf(stderr, "Error: %s", SDL_GetError());
        return;
    }

    // GL Rendering context creation
    context_ = SDL_GL_CreateContext(window_);
    if (!context_) {
        fprintf(stderr, "Error: %s", SDL_GetError());
        return;
    }
}

void Window::update() {
    // render
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, w_, h_);

    for (std::list<GuiComponentBase*>::iterator i = guiComponents_.begin(); i != guiComponents_.end(); i++) {
        (*i)->render();
    }

    // swap
    SDL_GL_SwapWindow(window_);
}

int Window::getW() {
    return w_;
}

int Window::getH() {
    return h_;
}

int Window::getX() {
    return x_;
}

int Window::getY() {
    return y_;
}

void Window::addComponent(GuiComponentBase* guiComponent) {
    guiComponents_.push_back(guiComponent);
    guiComponent->setWindow(this);
}
