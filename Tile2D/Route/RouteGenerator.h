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

#ifndef __RouteGenerator_H_
#define __RouteGenerator_H_

#include <queue>
#include "Point.h"

class GameWorld;
class Node;
class RouteRequest;
class RouteResponse;

class RouteGenerator {
    friend class Node;
    friend class GameWorld;
private:
    int maxGeneratingTimeMilliSec;

    GameWorld* world;
    std::queue<RouteRequest*> routeRequestQueue;

    RouteResponse* generateRoute(RouteRequest* request);
    int heuristicFunction(Node *startNode, Node *goalNode);

    void handleNextRequest();

    RouteGenerator();
public:
    void sendRequest(RouteRequest *request);
    void setMaxGeneratingTimeMilliSec(int timeMilliSec);
};


#endif //__RouteGenerator_H_
