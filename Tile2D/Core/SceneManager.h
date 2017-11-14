//
// Created by Miika Pelkonen on 10/11/2017.
//

#ifndef __SCENEMANAGER_H
#define __SCENEMANAGER_H

#include "IScene.h"

class SceneManager {
    friend class Tile2D;

public:
    void loadScene(int scene);

private:
    SceneManager();
    ~SceneManager();

    void init(std::map<int, IScene*>& scenes);

    std::map<int, IScene*> scenes_;
};


#endif //__SCENEMANAGER_H
