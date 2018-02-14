//
// Created by Miika Pelkonen on 15/12/2017.
//

#ifndef __utils_H
#define __utils_H

#include "precompile.h"
#include "Canvas.h"
#include "Camera.h"
#include "Tile2D.h"
#include "Vec.h"
#include "LineSegment.h"
#include "PolygonCollider.h"

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
