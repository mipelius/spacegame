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
#include "Map.h"

Node::Node(int x, int y) {
    this->x = x;
    this->y = y;
    this->nextNode = nullptr;
    this->previousNode = nullptr;
    this->fCost = 0;
    this->gCost = 0;
    this->hCost = 0;
}

Node *Node::byPoint(Point point, int blockSizeW, int blockSizeH) {
    int x = (int)point.x;
    x /= blockSizeW;
    int y = (int)point.y;
    y /= blockSizeH;
    return new Node(x, y);
}

Point Node::getLocation() {
    int w = generator->map->getBlockW();
    int h = generator->map->getBlockH();
    return Point(x * w + w / 2, y * h + h / 2);
}

bool Node::equals(Node *anotherNode) {
    return this->x == anotherNode->x && this->y == anotherNode->y;
}

Node *Node::getPreviousNode() {
    return this->previousNode;
}

Node *Node::getNextNode() {
    return this->nextNode;
}
