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

#ifndef __Plot_H_
#define __Plot_H_

template <typename T> class Property;
class Canvas;

#include "Point.h"
#include "IDrawable.h"

class Plot : public IDrawable {

public:
    Plot();
    ~Plot();

    Property<Point>* const location;
    Property<double>* const size;

    virtual void draw(Canvas *canvas);

private:
    Point location_;
    double size_;

};

#endif //__Plot_H_
