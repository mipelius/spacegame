//
// Created by Miika Pelkonen on 12/11/2017.
//

#ifndef __CONFIG_H
#define __CONFIG_H

#include <string>
#include "IScene.h"

class Config {

public:
    explicit Config(std::string configJsonFile);
    int window_x;
    int window_y;
    int window_w;
    int window_h;
    bool window_enableFullScreen;
};

#endif //__CONFIG_H
