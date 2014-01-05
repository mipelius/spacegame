//
// Created by Miika Pelkonen on 1/4/14.
// Copyright (c) 2014 ___MIIKAPELKONEN___. All rights reserved.
//

#include "GameEntity.h"

void GameEntity::applyForce(Vector force) {

}

void GameEntity::setAngle(double angle) {

}

Point GameEntity::getFocus() {
    return Point(0, 0);
}

void GameEntity::setFocus(Point location) {

}

Point GameEntity::getLocation() {
    return Point(0, 0);
}

void GameEntity::setLocation(Point location) {

}

void GameEntity::turnCounterClockwise() {

}

void GameEntity::turnClockwise() {

}

double GameEntity::getAngle() {
    return 0;
}

GameEntity::GameEntity(Point focus, Point location):focus(focus), location(location) {

}
