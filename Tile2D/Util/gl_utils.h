//
// Created by Miika Pelkonen on 15/12/2017.
//

#ifndef __utils_H
#define __utils_H

#include <SDL_opengl.h>
#include "Vec.h"
#include "LineSegment.h"
#include "PolygonCollider.h"

static void plot(const Vec& vec, double size = 5.0) {
    glBegin(GL_QUADS);
    glVertex2f(vec.x - size, vec.y - size);
    glVertex2f(vec.x + size, vec.y - size);
    glVertex2f(vec.x + size, vec.y + size);
    glVertex2f(vec.x - size, vec.y + size);
    glEnd();
}

static void drawLine(const LineSegment& lineSegment) {
    glBegin(GL_LINES);
    glVertex2d(lineSegment.point1.x, lineSegment.point1.y);
    glVertex2d(lineSegment.point2.x, lineSegment.point2.y);
    glEnd();
}

static void drawPolygonCollider(const PolygonCollider& polygonCollider, bool fill = false) {
    const auto& points = polygonCollider.points;
    if (fill)
        glBegin(GL_QUADS);
    else
        glBegin(GL_LINES);

    for (auto i = 0u; i < points.size(); i++) {
        Vec startPoint = points[i];
        startPoint.rotate(polygonCollider.rot);
        startPoint += polygonCollider.pos;

        Vec endPoint = points[(i+1) % points.size()];
        endPoint.rotate(polygonCollider.rot);
        endPoint += polygonCollider.pos;

        glVertex2d(startPoint.x, startPoint.y);
        glVertex2d(endPoint.x, endPoint.y);
    }
    glEnd();
}

static void prepareRendering() {
    Canvas& canvas = Tile2D::canvas();
    auto& camera = Tile2D::canvas().getCamera();
    Rect rect = canvas.getRenderingAreaRect();

    auto x = (GLint)(rect.x1);
    auto y = (GLint)(Tile2D::window().h.get() - rect.y2);
    auto w = (GLint)(rect.getWidth());
    auto h = (GLint)(rect.getHeight());

    glViewport(x, y, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(
            camera.areaRect.get().x1,
            camera.areaRect.get().x2,
            camera.areaRect.get().y2,
            camera.areaRect.get().y1,
            -1.0,
            1.0
    );
    glMatrixMode(GL_MODELVIEW);

}

#endif //__utils_H
