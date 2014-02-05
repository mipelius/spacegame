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

#ifndef __CoreRenderer_H_
#define __CoreRenderer_H_

#include <list>
#include <SDL2/SDL_video.h>

class IRenderer;
class Camera;
class RendererBase;

class CoreRenderer {
public:
    CoreRenderer();
    ~CoreRenderer();
    void initialize(int x, int y, int w, int h, bool enableFullScreen);

    void setCamera(Camera* camera);
    Camera* getCamera();

    void addRenderer(IRenderer* renderer);

    void render();
    void glSwap();

private:
    std::list<IRenderer*> renderers_;
    Camera* camera_;
    bool isInitialized_;

    SDL_Window *window_;
    SDL_GLContext context_;
};

#endif //__CoreRenderer_H_
