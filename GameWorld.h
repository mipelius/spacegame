//
// Created by Miika Pelkonen on 1/4/14.
// Copyright (c) 2014 ___MIIKAPELKONEN___. All rights reserved.
//


#import <list>
#include "GameEntity.h"
#include "Map.h"

#ifndef __GameWorld_H_
#define __GameWorld_H_


class GameWorld {
private:
    std::list<GameEntity> gameEntities;
    Map map;
public:
    void step(double time);
    void addObject(GameEntity gameObject);
    void setMap(Map map);
};

#endif //__GameWorld_H_
