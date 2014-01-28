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

#ifndef __RouteRequest_H_
#define __RouteRequest_H_

class Node;
class RouteResponse;
class RouteRequestSender;

#include "Point.h"
#include "Rect.h"

class RouteRequest {
    friend class RouteRequestSender;
    friend class RouteGenerator;
private:
    Point goalPoint;
    unsigned int step;
    Rect minSpace;
    RouteRequestSender* sender;
public:
    RouteRequest(Point goalPoint, unsigned int step, Rect minSpace);
};


#endif //__RouteRequest_H_
