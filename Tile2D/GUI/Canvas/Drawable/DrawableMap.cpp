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

#include "precompile.h"
#include "DrawableMap.h"
#include "MapTexture.h"
#include "TileMap.h"
#include "Camera.h"
#include "Canvas.h"

DrawableMap::DrawableMap() {
    mapTexture_ = nullptr;
    map_ = nullptr;
}

void DrawableMap::draw(const Canvas &canvas) {
    if (map_ == nullptr) return;

    double blockSizeW =
            canvas.getRenderingAreaRect().getWidth() /
            canvas.getCamera()->areaRect.get().getWidth() *
            map_->getBlockW();

    double blockSizeH =
            canvas.getRenderingAreaRect().getHeight() /
            canvas.getCamera()->areaRect.get().getHeight() *
            map_->getBlockH();

    if (blockSizeW > 1.0 && blockSizeH > 1.0) {
        drawMap(canvas);
    }
    else {
        drawSmallMap(canvas);
    }
}

void DrawableMap::setMap(TileMap *map) {
    map_ = map;
}

void DrawableMap::setMapTexture(MapTexture *mapTexture) {
    mapTexture_ = mapTexture;
}

void DrawableMap::drawMap(const Canvas &canvas) {
    const Rect& rect = canvas.getCamera()->areaRect.get();

    const double& x = rect.x1;
    const double& y = rect.y1;
    const double& w = rect.getWidth();
    const double& h = rect.getHeight();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    int iStart = 0;
    int jStart = 0;

    if (x > 0) iStart = int(x / map_->getBlockW());
    if (y > 0) jStart = int(y / map_->getBlockH());

    if (iStart < 0) iStart = 0;
    if (jStart < 0) jStart = 0;

    int iEnd = map_->getW();
    int jEnd = map_->getH();

    if (x + w < map_->getW() * map_->getBlockW()) iEnd = int((x + w) / map_->getBlockW() + 1);
    if (y + h < map_->getH() * map_->getBlockH()) jEnd = int((y + h) / map_->getBlockH() + 1);

    // NOTE: the textureNumber that is used for the current block is return value from getValue(i, j) - 1;
    // Maybe you should use more sophisticated way to indicate the correspondence between mapValues and textureNumbers

    mapTexture_->glBind();

    glColor3d(1.0, 1.0, 1.0);

    glBegin(GL_QUADS);

    for (int i = iStart; i < iEnd; i++) {
        for (int j = jStart; j < jEnd; j++) {
            Tile* block = map_->getValue(i, j);

            if (block == nullptr || block->getMapTextureId() == -1) continue;

            mapTexture_->glVertices(
                    i * map_->getBlockW(),
                    j * map_->getBlockH(),
                    map_->getBlockW(),
                    map_->getBlockH(),
                    block->getMapTextureId(),
                    MapTexture::CORNER_ROUNDING_NONE
            );
        }
    }

    glEnd();

    mapTexture_->glUnbind();
}

void DrawableMap::drawSmallMap(const Canvas& canvas) {
    Rect cameraRect = canvas.getCamera()->areaRect.get();
    Rect renderingAreaRect = canvas.getRenderingAreaRect();

    double stepX = cameraRect.getWidth() / renderingAreaRect.getWidth();
    double stepY = cameraRect.getHeight() / renderingAreaRect.getHeight();

    glBegin(GL_QUADS);
    for (double x = cameraRect.x1; x < cameraRect.x2; x += stepX) {
        for (double y = cameraRect.y1; y < cameraRect.y2; y += stepY) {
            Tile* block = map_->getValueScaled(
                    Vec(x, y)
            );

            if (block == nullptr || block->getMapTextureId() == -1) continue;

            glVertex2d(x, y);
            glVertex2d(x + stepX, y);
            glVertex2d(x + stepX, y + stepY);
            glVertex2d(x, y + stepY);

        }
    }
    glEnd();

}
