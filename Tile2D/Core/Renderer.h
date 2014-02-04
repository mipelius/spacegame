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

#include "Rect.h"

#ifndef __Renderer_H_
#define __Renderer_H_

class GameWorld;
class Background;
class Camera;

class Renderer {
private:
    SDL_Window *window_;
    SDL_GLContext context_;

    bool isInitialized_;
    Camera* camera_;
    GameWorld* gameWorld_;
    bool collisionShapesAreVisible_;
    bool mapCollisionAreaIsVisible_;
    void renderBackground();
    void renderBackground(Background* background, double opacity);
    void renderMap();
    void renderEntities();

    Background* oldBackground_;
    Background* background_;

    int backgroundCrossFadingPhase;
    bool backgroundIsCrossFading;

    Rect smallMapRenderingAreaRect;
    double smallMapHeight;
    double smallMapWidth;

    static const int BACKGROUND_CROSSFADE_RENDER_COUNT = 60;

public:
    Renderer();
    ~Renderer();
    void showCollisionShapes();
    void hideCollisionShapes();
    void toggleCollisionShapesVisibility();
    void showMapCollisionArea();
    void hideMapCollisionArea();
    void toggleMapCollisionAreaVisibility();
    void render();
    void init(int x, int y, int w, int h, bool enableFullScreen = false);
    void setGameWorld(GameWorld* gameWorld);
    GameWorld* getGameWorld();
    Camera* getCamera();
    void setBackground(Background *background);
    void glSwap();

    void setSmallMap(Rect renderingAreaRect, double mapWidth, double mapHeight);
};


#endif //__Renderer_H_
