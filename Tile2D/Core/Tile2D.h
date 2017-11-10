//
// Created by Miika Pelkonen on 10/11/2017.
//

#ifndef __TILE2D_H
#define __TILE2D_H

#include "SceneManager.h"

class Tile2D {

public:
    Tile2D(Tile2D const &) = delete;
    void operator=(Tile2D const &)  = delete;

    static SceneManager &sceneManager();

private:
    SceneManager *sceneManager_;

    Tile2D();
    ~Tile2D();

    static Tile2D &instance_();
};

#endif //__TILE2D_H
