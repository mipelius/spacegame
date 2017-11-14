//
// Created by Miika Pelkonen on 10/11/2017.
//

#ifndef __TILE2D_H
#define __TILE2D_H

#include <map>
#include "SceneManager.h"
#include "Resources.h"
#include "Window.h"

class Tile2D {

public:
    Tile2D(Tile2D const &) = delete;
    void operator=(Tile2D const &)  = delete;

    void static load(
            const std::string& configFile,
            std::vector<std::string> resTextures,
            std::vector<std::string> resSfx,
            std::vector<std::string> resMusic,
            std::vector<std::string> resFonts,
            std::vector<std::string> resBlockMappings,
            std::map<int, IScene*> scenes
    );

    static Window &window();
    static Resources &resources();
    static SceneManager &sceneManager();

private:
    Window *window_;
    Resources *resources_;
    SceneManager *sceneManager_;

    void mainLoop_();

    Tile2D();
    ~Tile2D();

    static Tile2D &instance_();
    static bool isLoaded_;
};

#endif //__TILE2D_H
