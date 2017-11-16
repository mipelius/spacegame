// This file is part of SpaceGame.
// Copyright (C) 2014 Miika Pelkonen
//
// SpaceGame is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// SpaceGame is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SpaceGame.  If not, see <http://www.gnu.org/licenses/>.

#include "precompile.h"
#include "Node.h"
#include "RouteGenerator.h"
#include "WorldMap.h"

Node::Node(int x, int y, WorldMap * map) {
    this->x = x;
    this->y = y;
    this->nextNode = nullptr;
    this->previousNode = nullptr;
    this->fCost = 0;
    this->gCost = 0;
    this->hCost = 0;
    this->map = map;
}

Node *Node::byPoint(Vector point, WorldMap * map) {
    int x = (int)point.x;
    x /= map->getBlockW();
    int y = (int)point.y;
    y /= map->getBlockH();
    return new Node(x, y, map);
}

Vector Node::getLocation() {
    int w = map->getBlockW();
    int h = map->getBlockH();
    return Vector(x * w + w / 2, y * h + h / 2);
}

bool Node::equals(Node *anotherNode, unsigned int rounding) {
    return this->x / rounding == anotherNode->x / rounding && this->y / rounding == anotherNode->y / rounding;
}

Node *Node::getPreviousNode() {
    return this->previousNode;
}

Node *Node::getNextNode() {
    return this->nextNode;
}
