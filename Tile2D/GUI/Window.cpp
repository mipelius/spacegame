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

#include <Tile2D/Util/JsonFileManager.h>
#include "precompile.h"
#include "Window.h"
#include "GuiComponentBase.h"

Window::Window() :
    position        (   Property<Point>            (this, getPosition_, setPosition_         )),
    isFullScreen    (   BooleanProperty            (this, getIsFullScreen_, setIsFullScreen_ )),
    w               (   ReadableProperty<unsigned> (&w_                                      )),
    h               (   ReadableProperty<unsigned> (&h_                                      ))
{
    isInitialized_ = false;
}

Window::~Window() {
    if (window_ == nullptr) {
        SDL_DestroyWindow(window_);
    }
}

void Window::init(const std::string& configJson) {
    if (isInitialized_) return;

    json::Object obj = JsonFileManager::load(configJson);
    json::Object windowJson = obj["window"];

    x_ = (unsigned)windowJson["x"].ToInt();
    y_ = (unsigned)windowJson["y"].ToInt();
    w_ = (unsigned)windowJson["w"].ToInt();
    h_ = (unsigned)windowJson["h"].ToInt();

    isFullScreen_ = windowJson["fullscreen"].ToBool();

    window_ = SDL_CreateWindow(
            "",
            x_,
            y_,
            w_,
            h_,
            SDL_WINDOW_OPENGL | (isFullScreen_ ? SDL_WINDOW_FULLSCREEN : 0)
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

    isFullScreen.updateDependentProperties();

    isInitialized_ = true;
}

void Window::update_() {
    // render
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, (GLsizei)w_, (GLsizei)h_);

    for (auto& guiComponent : guiComponents_) {
        guiComponent->render();
    }

    // swap
    SDL_GL_SwapWindow(window_);
}

void Window::addComponent(GuiComponentBase& guiComponent) {
    guiComponents_.push_back(&guiComponent);
    guiComponent.setWindow(this);
}

void Window::setSize(unsigned w, unsigned h) {
    SDL_SetWindowSize(window_, w, h);
    w_ = w;
    h_ = h;

    this->w.updateDependentProperties();
    this->h.updateDependentProperties();
}

Point Window::getPosition_(void *owner) {
    auto window = (Window*)owner;
    return Point(window->x_, window->y_);
}

void Window::setPosition_(void *owner, const Point &value) {
    auto window = (Window*)owner;
    SDL_SetWindowPosition(window->window_, (int)value.x, (int)value.y);
    window->x_ = value.x;
    window->y_ = value.y;
}


bool Window::getIsFullScreen_(void *owner) {
    return ((Window*)owner)->isFullScreen_;
}
void Window::setIsFullScreen_(void *owner, const bool &value) {
    auto window = (Window*)owner;

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