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


#include <SDL_video.h>
#include "Window.h"
#include "Camera.h"
#include "JsonFileManager.h"
#include "Tile2D.h"
#include "precompile.h"
#include "LightSystem.h"

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

    settingsFilePath_ = configJson;

    json::Object obj;
    bool loadingFailed = false;

    try {
        obj = JsonFileManager::load(settingsFilePath_);
    } catch (const std::runtime_error& error) {
        loadingFailed = true;
        json::Object window;
        window["use_default_fullscreen"] = true;
        window["vsync"] = true;
        obj["window"] = window;
    }

    json::Object windowJson = obj["window"];

    int x, y, w, h;

    if (windowJson["use_default_fullscreen"].ToBool()) {
        static int displayInUse = 0;
        SDL_DisplayMode mode;
        SDL_GetCurrentDisplayMode(displayInUse, &mode);
        x = 0;
        y = 0;
        w = (unsigned)mode.w;
        h = (unsigned)mode.h;

        isFullScreen_ = true;
    } else {
        x = (unsigned)windowJson["x"].ToInt();
        y = (unsigned)windowJson["y"].ToInt();
        w = (unsigned)windowJson["w"].ToInt();
        h = (unsigned)windowJson["h"].ToInt();

        isFullScreen_ = windowJson["fullscreen"].ToBool();
    }
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    window_ = SDL_CreateWindow(
            "",
            x,
            y,
            w,
            h,
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

    glViewport(x, y, w, h);

    isInitialized_ = true;

    if (loadingFailed) {
        saveSettings();
    }
}

void Window::swap_() {
    SDL_GL_SwapWindow(window_);
}

void Window::setSize(Veci size) {
    SDL_SetWindowSize(window_, size.x, size.y);
    glViewport(0, 0, size.x, size.y);
    Tile2D::lightSystem().init();
}

Veci Window::getSize() {
    int w, h;
    SDL_GetWindowSize(window_, &w, &h);
    return { w, h };
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

std::vector<Veci> Window::getAllowedFullScreenResolutions() {
    std::vector<Veci> result;

    auto num = SDL_GetNumDisplayModes(0);

    for (auto i = 0; i < num; ++i) {
        SDL_DisplayMode mode;
        SDL_GetDisplayMode(0, i, &mode);

        result.emplace_back(mode.w, mode.h);
    }

    return result;
}

void Window::saveSettings() {
    json::Object window;

    auto windowSize = getSize();

    window["use_default_fullscreen"] = false;
    window["x"] = 0;
    window["y"] = 0;
    window["w"] = windowSize.x;
    window["h"] = windowSize.y;
    window["fullscreen"] = true;
    window["vsync"] = true;

    json::Object data;
    data["window"] = window;

    JsonFileManager::save(data, settingsFilePath_);
}
