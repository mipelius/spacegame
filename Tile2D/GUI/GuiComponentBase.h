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

#ifndef __GuiComponentBase_H_
#define __GuiComponentBase_H_

class Window;

#include <list>
#include "Rect.h"

class GuiComponentBase {
    friend class Window;
    friend class GuiComponentBase;
public:
    enum Anchor {TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT};
    static const int SIZE_MAX_WIDTH = -1;
    static const int SIZE_MAX_HEIGHT = -1;

    GuiComponentBase();

    virtual void render();

    Rect getRenderingAreaRect();

    void setMargin(double top, double right, double bottom, double left);
    void setAnchor(Anchor anchor);

    void addComponent(GuiComponentBase* guiComponent);

    Window* getWindow();

    void setW(double w);
    void setH(double h);

    void hide();
    void show();
    void toggleVisibility();

protected:
    std::list<GuiComponentBase*> children_;

private:
    void setWindow(Window* window);
    Window* window_;
    GuiComponentBase* parentGuiComponent_;
    Anchor anchor_;

    bool isVisible_;

    double w_;
    double h_;

    double marginTop_;
    double marginBottom_;
    double marginLeft_;
    double marginRight_;

    void setParent(GuiComponentBase *guiComponent);
};


#endif //__GuiComponentBase_H_
