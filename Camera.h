//
// Created by Miika Pelkonen on 1/4/14.
// Copyright (c) 2014 ___MIIKAPELKONEN___. All rights reserved.
//


#include "GameObject.h"

#ifndef __Camera_H_
#define __Camera_H_


class Camera {
private:
    GameObject objectToFollow;
    long x, y, w, h;
public:
    void follow(GameObject gameObject);
};


#endif //__Camera_H_
