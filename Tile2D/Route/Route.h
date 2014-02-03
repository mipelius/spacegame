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

#ifndef __Route_H_
#define __Route_H_

class Node;

class Route {
    friend class RouteGenerator;
private:
    Node* _firstNode;
    Node* _lastNode;
    Node* _currentNode;
    Route(Node* firstNode, Node* lastNode);
public:
    ~Route();

    void begin();
    void end();
    void nextNode();
    void previousNode();
    Node* getCurrentNode();
};


#endif //__Route_H_
