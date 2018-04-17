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

#ifndef __utils_H
#define __utils_H

#include "precompile.h"
#include "Canvas.h"
#include "Camera.h"
#include "Tile2D.h"
#include "Vec.h"
#include "LineSegment.h"
#include "PolygonCollider.h"
#include "Window.h"

static void plot(const Vecf& vec, float size = 5.0) {
    glBegin(GL_QUADS);
    glVertex2f(vec.x - size, vec.y - size);
    glVertex2f(vec.x + size, vec.y - size);
    glVertex2f(vec.x + size, vec.y + size);
    glVertex2f(vec.x - size, vec.y + size);
    glEnd();
}

static void drawLine(const LineSegment& lineSegment) {
    glBegin(GL_LINES);
    glVertex2f(lineSegment.point1.x, lineSegment.point1.y);
    glVertex2f(lineSegment.point2.x, lineSegment.point2.y);
    glEnd();
}

static void prepareRendering() {
    Canvas& canvas = Tile2D::canvas();
    auto camera = Tile2D::canvas().getCamera();
    Rect rect = Tile2D::window().getRect();

    auto x = (GLint)(rect.x1);
    auto y = (GLint)(Tile2D::window().getH() - rect.y2);
    auto w = (GLint)(rect.getWidth());
    auto h = (GLint)(rect.getHeight());

    const Rect& cameraAreaRect = camera->getAreaRect();

    glViewport(x, y, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(
            cameraAreaRect.x1,
            cameraAreaRect.x2,
            cameraAreaRect.y2,
            cameraAreaRect.y1,
            -1.0,
            1.0
    );
}

#endif //__utils_H
