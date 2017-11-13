//
// Created by Miika Pelkonen on 10/11/2017.
//

#ifndef __TILE2D_H
#define __TILE2D_H

#include <map>
#include "SceneManager.h"
#include "Resources.h"
#include "Config.h"

class Tile2D {

public:
    Tile2D(Tile2D const &) = delete;
    void operator=(Tile2D const &)  = delete;

    void static load(Config &config, Resources &resources, std::map<int, IScene*> scenes);

    static SceneManager &sceneManager();

private:
    SceneManager *sceneManager_;

    Tile2D();
    ~Tile2D();

    static Tile2D &instance_();
    static bool isLoaded_;
};

#endif //__TILE2D_H
