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
#include "TileSet.h"
#include "Camera.h"
#include "Canvas.h"

DrawableMap::DrawableMap() {
    mapTexture_ = nullptr;
    map_ = nullptr;
}

void DrawableMap::draw(const Canvas &canvas) {
    if (map_ == nullptr || canvas.getCamera() == nullptr) return;

    const Rect& rect = canvas.getCamera()->areaRect.get();

    const double& x = rect.x1;
    const double& y = rect.y1;
    const double& w = rect.getWidth();
    const double& h = rect.getHeight();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    int iStart = 0;
    int jStart = 0;

    if (x > 0) iStart = int(x / map_->getTileSet()->getTileW());
    if (y > 0) jStart = int(y / map_->getTileSet()->getTileH());

    if (iStart < 0) iStart = 0;
    if (jStart < 0) jStart = 0;

    int iEnd = map_->getW();
    int jEnd = map_->getH();

    if (x + w < map_->getW() * map_->getTileSet()->getTileW()) iEnd = int((x + w) / map_->getTileSet()->getTileW() + 1);
    if (y + h < map_->getH() * map_->getTileSet()->getTileH()) jEnd = int((y + h) / map_->getTileSet()->getTileH() + 1);

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
                    i * map_->getTileSet()->getTileW(),
                    j * map_->getTileSet()->getTileH(),
                    map_->getTileSet()->getTileW(),
                    map_->getTileSet()->getTileH(),
                    block->getMapTextureId(),
                    MapTexture::CORNER_ROUNDING_NONE
            );
        }
    }

    glEnd();

    mapTexture_->glUnbind();
}

void DrawableMap::setMap(TileMap *map) {
    map_ = map;
}

void DrawableMap::setMapTexture(MapTexture *mapTexture) {
    mapTexture_ = mapTexture;
}

void DrawableMap::drawMap(const Canvas &canvas) {

}
