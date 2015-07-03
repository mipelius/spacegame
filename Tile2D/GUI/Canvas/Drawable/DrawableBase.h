// This file is part of SpaceGame.
// Copyright (C) 2015 Miika Pelkonen
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

#ifndef __DrawableBase_H_
#define __DrawableBase_H_

#include "IDrawable.h"
#include "Canvas.h"
#include "Property.h"
#include "Color.h"

class DrawableBase : public IDrawable{

public:
    DrawableBase();
    ~DrawableBase();

    Property<Point>     const location;
    Property<double>    const angle;
    BooleanProperty     const isVisible;
    Property<double>    const opacity;
    Property<Color>     const color;

    virtual void draw(Canvas *canvas);

protected:
    virtual void drawActual(Canvas* canvas) = 0;

private:
    Point location_;
    double angle_;
    bool isVisible_;
    double opacity_;
    Color color_;
};


#endif //__DrawableBase_H_
