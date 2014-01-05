//
// Created by Miika Pelkonen on 1/4/14.
// Copyright (c) 2014 ___MIIKAPELKONEN___. All rights reserved.
//

#include "Block.h"

Block::Block(std::string name) {

}

std::string Block::getName() {
    return this->name;
}

void Block::setName(std::string name) {
    this->name = name;
}
