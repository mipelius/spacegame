//
// Created by Miika Pelkonen on 10/11/2017.
//

#ifndef __SCENEMANAGER_H
#define __SCENEMANAGER_H

#include "IScene.h"

class SceneManager {

public:
    void loadScene(int scene);

private:
    SceneManager();
    ~SceneManager();

    std::map<int, IScene*> scenes_;

    friend class Tile2D;

};


#endif //__SCENEMANAGER_H
