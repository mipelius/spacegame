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


#ifndef __Canvas_H_
#define __Canvas_H_

#include <list>
#include <string>
#include <map>

class Camera;
class DrawableBase;

struct SortingLayer {
    int         id;
    std::string name;
    int         order;
};

class Canvas {
    friend class Tile2D;
    friend class DrawableBase;

public:
    void setCamera(Camera* camera);
    Camera* getCamera() const;

    const std::map<int, SortingLayer>& getSortingLayers();

private:
    Canvas();

    void render();
    void addDrawable(DrawableBase *drawable);
    void removeDrawable(DrawableBase* drawable);

    void setCameraProjection_();
    void setUIProjection_();
    void sortDrawables_();
    void renderDrawablesBeforeTerrain_(std::list<DrawableBase*>::iterator& it);
    void renderDrawablesAfterTerrain_(std::list<DrawableBase*>::iterator& it);
    void renderUIDrawables_(std::list<DrawableBase*>::iterator& it);

    void renderDrawable_(DrawableBase* drawable);

    void init(const std::string& sortingLayersFile);

    std::map<int, SortingLayer> sortingLayers_;
    std::list<DrawableBase*> drawables_;
    Camera* camera_;
};

#endif //__Canvas_H_
