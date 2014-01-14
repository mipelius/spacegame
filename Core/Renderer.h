// This file is part of SpaceGame.
// Copyright (C) 2014  Miika Pelkonen
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

#include "GameWorld.h"
#include "Background.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#ifndef __Renderer_H_
#define __Renderer_H_

#include "Camera.h"

class Camera;

class Renderer {
private:
    SDL_Window *window;
    SDL_GLContext context;
    bool isInitialized;
    Camera* camera;
    GameWorld* gameWorld;
    std::list<Background*> *backgrounds;
    bool collisionShapesAreVisible;
    void renderBackground();
    void renderMap();
    void renderEntities();

public:
    Renderer();
    ~Renderer();
    void showCollisionShapes();
    void hideCollisionShapes();
    void toggleCollisionShapesVisibility();
    void render();
    void init(int x, int y, int w, int h, bool enableFullScreen = false);
    void setGameWorld(GameWorld* gameWorld);
    GameWorld* getGameWorld();
    Camera* getCamera();
    void addBackground(Background *background);
};


#endif //__Renderer_H_
