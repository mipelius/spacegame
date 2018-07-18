// MIT License
//
// This file is part of SpaceGame.
// Copyright (c) 2014-2018 Miika Pelkonen
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


#ifndef __Window_H_
#define __Window_H_

class GuiComponentBase;

#include "precompile.h"
#include <list>
#include "Vec.h"
#include "Rect.h"

class Window {
    friend class Tile2D;
public:

    // getters and setters
    void setSize(Veci size);
    Veci getSize();
    bool getVsync() const;
    void setVsync(bool vsync);

    std::vector<Veci> getAllowedFullScreenResolutions();

private:
    void swap_();

    ~Window();
    Window();
    void init(const std::string& configJson);

    SDL_Window* window_;
    SDL_GLContext context_;

    bool isFullScreen_;
    bool isInitialized_;
};

#endif //__Window_H_
