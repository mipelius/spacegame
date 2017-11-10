//
// Created by Miika Pelkonen on 10/11/2017.
//

#ifndef __SCENEMANAGER_H
#define __SCENEMANAGER_H

#include <iostream>

class SceneManager {

public:
    void loadScene(std::string sceneStr);

private:
    SceneManager();
    ~SceneManager();

    friend class Tile2D;

};


#endif //__SCENEMANAGER_H
