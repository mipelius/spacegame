//
// Created by Miika Pelkonen on 1/5/14.
// Copyright (c) 2014 ___MIIKAPELKONEN___. All rights reserved.
//

#include "CollisionShape.h"

CollisionShape::CollisionShape(Point **points) {
    this->points = points;
}

Point **CollisionShape::getPoints() {
    return this->points;
}
