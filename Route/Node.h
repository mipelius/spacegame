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

#ifndef __Node_H_
#define __Node_H_

#include <list>
#include "Point.h"

class Map;

class Node {
    friend class RouteGenerator;
    friend class Node;
private:
    Node* previousNode;
    Node* nextNode;
    Map* map;
    int x;
    int y;
    int fCost;
    int hCost;
    int gCost;
    Node(int x, int y, Map* map);
    static Node* byPoint(Point point, Map* map);
public:
    bool equals(Node* anotherNode);
    Point getLocation();
    Node* getPreviousNode();
    Node* getNextNode();

};

#endif //__Node_H_
