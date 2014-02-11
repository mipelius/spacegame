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

class Window {

public:
    static Window* getInstance();
    void initialize(int x, int y, int w, int h, bool enableFullScreen);
    ~Window();

    void update();
    int getW();
    int getH();
    int getX();
    int getY();

    void addComponent(GuiComponentBase* guiComponent);

private:
    Window();

    std::list<GuiComponentBase*> guiComponents_;

    static Window* instance_;

    SDL_Window* window_;
    SDL_GLContext context_;

    int w_;
    int h_;
    int x_;
    int y_;
};

#endif //__Window_H_
