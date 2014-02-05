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
#include "CoreRenderer.h"
#include "Camera.h"
#include "IRenderer.h"

CoreRenderer::CoreRenderer() {
    renderers_ = std::list<IRenderer*>();
    isInitialized_ = false;
}

CoreRenderer::~CoreRenderer() {

}

void CoreRenderer::initialize(int x, int y, int w, int h, bool enableFullScreen) {
    // Window initialization
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

    // GL projection matrix initialization
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, h, 0, -1, 1);
    glClearColor(0.0, 0.0, 0.0, 0.0);

    isInitialized_ = true;
}

void CoreRenderer::setCamera(Camera* camera) {
    camera_ = camera;
}

Camera *CoreRenderer::getCamera() {
    return camera_;
}

void CoreRenderer::addRenderer(IRenderer *renderer) {
    if (!renderer->getCamera()) renderer->setCamera(camera_);
    renderers_.push_back(renderer);
}

void CoreRenderer::render() {
    if (!isInitialized_) {
        fprintf(stderr, "Error: renderer is not initialized.");
        return;
    }

    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

    for (std::list<IRenderer*>::iterator i = renderers_.begin(); i != renderers_.end(); i++) {
        (*i)->render();
    }
}

void CoreRenderer::glSwap() {
    SDL_GL_SwapWindow(window_);
}
