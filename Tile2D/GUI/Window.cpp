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
#import "SimpleReadableProperty.h"

class Window::PropertyLocation : public Property<Point> {

public:
    PropertyLocation(Window *actualData):Property(actualData) { }

    Point getActual() {
        Window* window = (Window*)actualData_;
        return Point(window->x_, window->y_);
    }

    void setActual(Point point) {
        Window* window = (Window*)actualData_;
        SDL_SetWindowPosition(window->window_, (int)point.x, (int)point.y);
        window->x_ = point.x;
        window->y_ = point.y;
    }
};

class Window::PropertyIsFullScreen : public Property<bool> {

public:
    PropertyIsFullScreen(Window *actualData):Property(actualData) { }

    bool getActual() {
        return ((Window*)actualData_)->isFullScreen_;
    }

    void setActual(bool value) {
        Window* window = (Window*)actualData_;

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
};

Window* Window::instance_ = nullptr;

Window::Window() :
    location        (   new PropertyLocation                (this   )   ),
    isFullScreen    (   new PropertyIsFullScreen            (this   )   ),
    w               (   new SimpleReadableProperty<double>  (&w_    )   ),
    h               (   new SimpleReadableProperty<double>  (&h_    )   )
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
    isFullScreen->updateDependentProperties();

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

void Window::addComponent(GuiComponentBase* guiComponent) {
    guiComponents_.push_back(guiComponent);
    guiComponent->setWindow(this);
}

void Window::setSize(double w, double h) {
    SDL_SetWindowSize(window_, (int)w, (int)h);
    w_ = w;
    h_ = h;

    this->w->updateDependentProperties();
    this->h->updateDependentProperties();
}
