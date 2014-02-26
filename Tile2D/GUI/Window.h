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

#ifndef __Window_H_
#define __Window_H_

class GuiComponentBase;

#include <SDL2/SDL_video.h>
#include <list>
#include "Property.h"
#include "Point.h"

class Window {
    friend class PropertyLocation;
    friend class PropertyIsFullScreen;

public:

    static Window* getInstance();
    void initialize(int x, int y, int w, int h, bool enableFullScreen);
    ~Window();

    void update();

    Property<Point>* const location;
    Property<bool>* const isFullScreen;
    ReadableProperty<double>* const w;
    ReadableProperty<double>* const h;

    void setSize(double w, double h);

    void addComponent(GuiComponentBase* guiComponent);

private:
    Window();

    std::list<GuiComponentBase*> guiComponents_;

    static Window* instance_;

    SDL_Window* window_;
    SDL_GLContext context_;

    double w_;
    double h_;
    double x_;
    double y_;

    bool isFullScreen_;

    bool isInitialized_;

    class PropertyLocation;
    class PropertyIsFullScreen;
};

#endif //__Window_H_
