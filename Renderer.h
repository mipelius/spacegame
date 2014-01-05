//
// Created by Miika Pelkonen on 1/4/14.
// Copyright (c) 2014 ___MIIKAPELKONEN___. All rights reserved.
//


#include "Camera.h"
#include "GameWorld.h"

#ifndef __Renderer_H_
#define __Renderer_H_


class Renderer {
private:
    Camera camera;
    GameWorld gameWorld;
    void renderBackground();
    void renderMap();
    void renderEntities();
    void renderEntity(GameEntity gameEntity);
public:
    void render();
};


#endif //__Renderer_H_
