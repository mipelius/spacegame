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

#ifndef __Text_H_
#define __Text_H_

#include <string>
#include "IDrawable.h"
#include "Property.h"
#include "Point.h"
#include "DrawableBase.h"

class Font;

class Text : public DrawableBase {
public:
    Text(Font* font);
    ~Text();

    // properties

    Property<Point>* const location;
    Property<float>* const size;
    Property<std::string>* const string;

    virtual void drawActual(Canvas *canvas);

private:
    Font* font_;

    Point location_;
    float size_;
    std::string string_;

};


#endif //__Text_H_
