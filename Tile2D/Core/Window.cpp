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


#include "Window.h"
#include "Camera.h"
#include "JsonFileManager.h"
#include "Tile2D.h"
#include "precompile.h"

Window::Window()
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

    if (windowJson["use_default_fullscreen"].ToBool()) {
        static int displayInUse = 0;
        SDL_DisplayMode mode;
        SDL_GetCurrentDisplayMode(displayInUse, &mode);
        x_ = 0;
        y_ = 0;
        w_ = (unsigned)mode.w;
        h_ = (unsigned)mode.h;

        isFullScreen_ = true;
    } else {
        x_ = (unsigned)windowJson["x"].ToInt();
        y_ = (unsigned)windowJson["y"].ToInt();
        w_ = (unsigned)windowJson["w"].ToInt();
        h_ = (unsigned)windowJson["h"].ToInt();

        isFullScreen_ = windowJson["fullscreen"].ToBool();
    }
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

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

    setVsync(windowJson["vsync"]);

    isInitialized_ = true;
}

void Window::swap_() {
    SDL_GL_SwapWindow(window_);
}

void Window::setSize(unsigned w, unsigned h) {
    SDL_SetWindowSize(window_, w, h);
    w_ = w;
    h_ = h;
}

Vecf Window::getPosition_(void *owner) {
    auto window = (Window*)owner;
    return Vecf(window->x_, window->y_);
}

void Window::setPosition_(void *owner, const Vecf &value) {
    auto window = (Window*)owner;
    SDL_SetWindowPosition(window->window_, (int)value.x, (int)value.y);
    window->x_ = (unsigned)value.x;
    window->y_ = (unsigned)value.y;
}

// getters and setters

unsigned int Window::getW() const {
    return w_;
}

void Window::setW(unsigned int w) {
    w_ = w;
}

unsigned int Window::getH() const {
    return h_;
}

void Window::setH(unsigned int h) {
    h_ = h;
}

Rect Window::getRect() {
    return Rect(x_, y_, w_, h_);
}

bool Window::getVsync() const {
    return SDL_GL_GetSwapInterval() == 1;
}

void Window::setVsync(bool vsync) {
    if (vsync) {
        SDL_GL_SetSwapInterval(1);
    } else {
        SDL_GL_SetSwapInterval(0);
    }

}
