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

#include "Tile2DComponent.h"
#include "Canvas.h"
#include "Color.h"

class DrawableBase : public Tile2DComponent {
    friend class Canvas;
public:
    DrawableBase();

    void draw(const Canvas& canvas);
    // getters and setters

    bool isVisible() const;
    void setIsVisible(bool isVisible);
    float getOpacity() const;
    void setOpacity(float opacity);
    const Color &getColor() const;
    void setColor(const Color &color);
    int getSortingLayer() const;
    void setSortingLayer(int sortingLayer);

protected:
    virtual void drawActual(const Canvas& canvas) = 0;

    void init() override;
    void onDestroy() override;

private:
    bool isVisible_;
    float opacity_;
    Color color_;
    int sortingLayer_;
};


#endif //__DrawableBase_H_
