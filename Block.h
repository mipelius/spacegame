//
// Created by Miika Pelkonen on 1/4/14.
// Copyright (c) 2014 ___MIIKAPELKONEN___. All rights reserved.
//


#include <string>

#ifndef __Block_H_
#define __Block_H_


class Block {
private:
    std::string name;
    // Texture???
public:
    Block(std::string name /* Texture, texture?? */);
    std::string getName();
    void setName(std::string);
};


#endif //__Block_H_
