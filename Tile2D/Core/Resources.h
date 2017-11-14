//
// Created by Miika Pelkonen on 13/11/2017.
//

#ifndef __RESOURCES_H
#define __RESOURCES_H

#include <map>

class Resources {
    friend class Tile2D;
private:
    Resources() = default;
    ~Resources() = default;

    void init(
            std::map<std::string, std::string>& resTextures,
            std::map<std::string, std::string>& resSfx,
            std::map<std::string, std::string>& resMusic,
            std::map<std::string, std::string>& resFonts,
            std::map<std::string, std::string>& resBlockMappings
    );
};


#endif //__RESOURCES_H
