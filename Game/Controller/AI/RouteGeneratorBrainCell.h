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

#ifndef __RouteGeneratorBrainCell_H_
#define __RouteGeneratorBrainCell_H_

class RouteResponse;
class Route;

#include "Point.h"
#include "RouteRequestSender.h"
#include "BrainCell.h"

class RouteGeneratorBrainCell : public BrainCell, RouteRequestSender {
friend class RouteUpdaterRequest;
private:
    Route* _route;
    bool _isEnabled;
    Point _destination;

    Point getLocation();
    void handleResponse(RouteResponse * response);
    void removeRoute();
protected:
    void operate();
public:
    RouteGeneratorBrainCell(double tickSeconds);

    void enable();
    void disable();
    bool isEnabled();

    void setDestination(Point location);

    Route* getRoute();
};


#endif //__RouteGeneratorBrainCell_H_
