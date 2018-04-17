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

    const Rect& rect = canvas.getCamera()->getAreaRect();

    const float& x = rect.x1;
    const float& y = rect.y1;
    const float& w = rect.getWidth();
    const float& h = rect.getHeight();

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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0, 1.0, 1.0, 1.0);

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
