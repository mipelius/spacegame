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

Window::Window() :
    location        (   Property<Point>           (this, getLocation_, setLocation_         )   ),
    isFullScreen    (   BooleanProperty           (this, getIsFullScreen_, setIsFullScreen_ )   ),
    w               (   ReadableProperty<double>  (&w_    )   ),
    h               (   ReadableProperty<double>  (&h_    )   )
{
    isInitialized_ = false;
}

Window::~Window() {
    if (window_) SDL_DestroyWindow(window_);
}

Window* Window::getInstance() {
    if (!instance_) instance_ = new Window();
    return instance_;
}

void Window::initialize(int x, int y, int w, int h, bool enableFullScreen) {
    if (isInitialized_) return;

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

    isFullScreen_ = enableFullScreen;
    isFullScreen.updateDependentProperties();

    isInitialized_ = true;
}

void Window::update() {
    // render
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, (GLsizei)w_, (GLsizei)h_);

    for (std::list<GuiComponentBase*>::iterator i = guiComponents_.begin(); i != guiComponents_.end(); i++) {
        (*i)->render();
    }

    // swap
    SDL_GL_SwapWindow(window_);
}

void Window::addComponent(GuiComponentBase& guiComponent) {
    guiComponents_.push_back(&guiComponent);
    guiComponent.setWindow(this);
}

void Window::setSize(double w, double h) {
    SDL_SetWindowSize(window_, (int)w, (int)h);
    w_ = w;
    h_ = h;

    this->w.updateDependentProperties();
    this->h.updateDependentProperties();
}

Point Window::getLocation_(void *owner) {
    Window* window = (Window*)owner;
    return Point(window->x_, window->y_);
}

void Window::setLocation_(void *owner, const Point &value) {
    Window* window = (Window*)owner;
    SDL_SetWindowPosition(window->window_, (int)value.x, (int)value.y);
    window->x_ = value.x;
    window->y_ = value.y;
}


bool Window::getIsFullScreen_(void *owner) {
    return ((Window*)owner)->isFullScreen_;
}
void Window::setIsFullScreen_(void *owner, const bool &value) {
    Window* window = (Window*)owner;

    int retValue;

    if (value) {
        retValue = SDL_SetWindowFullscreen(window->window_, SDL_WINDOW_FULLSCREEN);
    }
    else {
        retValue = SDL_SetWindowFullscreen(window->window_, 0);
    }

    if (retValue == 0) {
        window->isFullScreen_ = value;
    }
}