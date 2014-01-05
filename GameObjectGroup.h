//
// Created by Miika Pelkonen on 1/4/14.
// Copyright (c) 2014 ___MIIKAPELKONEN___. All rights reserved.
//


#import <list>
#include "GameObject.h"

#ifndef __GameObjectGroup_H_
#define __GameObjectGroup_H_


class GameObjectGroup {
private:
    std::list<GameObject> gameObjects;
public:
    void add(GameObject gameObject);
};


#endif //__GameObjectGroup_H_
