// This file is part of SpaceGame.
// Copyright (C) 2014  Miika Pelkonen
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
#include "Map.h"
#include "Rect.h"
#include "MapTexture.h"
#include "GameEntity.h"
#include "CollisionShape.h"

Map::Map(
        std::string path,
        MapTexture* mapTexture,
        int blockSizeW,
        int blockSizeH
) {
    this->blockSizeW = blockSizeW;
    this->blockSizeH = blockSizeH;
    this->mapTexture = mapTexture;

    using namespace std;

    SDL_Surface* surface = IMG_Load(path.data());
    if (!surface) {
        std::fprintf(stderr, "Error during loading map: %s\n", SDL_GetError());
        exit(1);
    }

    Uint8* pixels = (Uint8*)surface->pixels;

    this->initialize(surface->w, surface->h);

    for (int i=0; i<surface->w; i++) {
        for (int j=0; j<surface->h; j++) {
            this->setValue(i, j, *(pixels + j * w + i));
        }
    }

    SDL_FreeSurface(surface);
}

void Map::initialize(int w, int h) {
    this->w = w;
    this->h = h;
    p_values = new unsigned char*[w];

    for (int i = 0; i < w; i++) {
        p_values[i] = new unsigned char[h];
        unsigned char *row = p_values[i];
        for (int j = 0; j < h;j++) {
            row[j] = 0;
        }
    }
}

void Map::setValue(int x, int y, unsigned char value) {
    if (x < 0 || x >= w || y < 0 || y >= h) return;
    unsigned char *row = p_values[x];
    row[y] = value;
}

unsigned char Map::getValue(int x, int y) {
    if (x < 0 || x >= w || y < 0 || y >= h) return 0;
    unsigned char *row = p_values[x];
    return row[y];
}

int Map::getW() {
    return w;
}

int Map::getH() {
    return h;
}

void Map::render(double x, double y, double w, double h) {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    int iStart = 0;
    int jStart = 0;

    if (x > 0) iStart = int(x / blockSizeW);
    if (y > 0) jStart = int(y / blockSizeH);

    if (iStart < 0) iStart = 0;
    if (jStart < 0) jStart = 0;

    int iEnd = this->getW();
    int jEnd = this->getH();

    if (x + w < this->getW() * blockSizeW) iEnd = int((x + w) / blockSizeW + 1);
    if (y + h < this->getH() * blockSizeH) jEnd = int((y + h) / blockSizeH + 1);

    // NOTE: the textureNumber that is used for the current block is return value from getValue(i, j) - 1;
    // Maybe you should use more sophisticated way to indicate the correspondence between mapValues and textureNumbers

    mapTexture->glBind();

    for (int i = iStart; i < iEnd; i++) {
        for (int j = jStart; j < jEnd; j++) {
            if (!this->getValue(i, j)) continue; // continue if the block is empty

            GLdouble color = sin(((i*blockSizeW - x) / (float)w) * (M_PI / 1.0)) * sin(((j*blockSizeH - y) / (float)h) * (M_PI / 1.0));
            color /= 1.5;
            color += 0.25;
            if (color > 1.0) color = 1.0;
            glColor4d(color, color, color, color);

            mapTexture->renderBlock(
                    (int)(i * blockSizeW - x),
                    (int)(j * blockSizeH - y),
                    blockSizeW,
                    blockSizeH,
                    this->getValue(i, j) - 1
            );
        }
    }
    mapTexture->glUnbind();

}

long Map::getActualW() {
    return w * blockSizeW;
}

long Map::getActualH() {
    return h * blockSizeH;
}

unsigned char Map::getValueActual(int x, int y) {
    return this->getValue(x / blockSizeW, y / blockSizeH);
}

void Map::setValueActual(int x, int y, unsigned char value) {
    this->setValue(x / blockSizeW, y / blockSizeH, value);
}

bool Map::detectCollisionWith(GameEntity *entity) {
    if (this->getValueActual(entity->getLocation().x, entity->getLocation().y)) return true;

    Point* points = entity->getCollisionShape()->getRotatedPoints();
    for (int i=0; i<entity->getCollisionShape()->getCount(); i++) {
        int x = (int)(points[i].x + entity->getLocation().x);
        int y = (int)(points[i].y + entity->getLocation().y);
        if (this->getValueActual(x, y)) return true;
    }

    Rect boundingBox = entity->getCollisionShape()->getBoundingBox();

    int iBegin = (int)boundingBox.getFirstPoint().x - ((int)boundingBox.getFirstPoint().x) % blockSizeW;
    int iEnd = (int)boundingBox.getSecondPoint().x + (int)boundingBox.getSecondPoint().x % blockSizeW;
    int jBegin = (int)boundingBox.getFirstPoint().y - ((int)boundingBox.getFirstPoint().y) % blockSizeH;
    int jEnd = (int)boundingBox.getSecondPoint().y + (int)boundingBox.getSecondPoint().y % blockSizeH;

    for (int i=iBegin; i <= iEnd; i += blockSizeW) {
        for (int j=jBegin; j <= jEnd ; j += blockSizeH) {
            if (this->getValueActual(i, j)) {
                Rect* rect = new Rect(Point(i, j), Point(i + blockSizeW, j + blockSizeH));
                if (entity->getCollisionShape()->intersectsWith(rect)) {
                    return true;
                }
            }
        }
    }

    return false;
}

int Map::getBlockW() {
    return this->blockSizeW;
}


int Map::getBlockH() {
    return this->blockSizeH;
}

void Map::renderSmall(Rect mapRect, Rect renderingAreaRect) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    double opacity = 0.4;

    double stepX = mapRect.getWidth() / renderingAreaRect.getWidth();
    double stepY = mapRect.getHeight() / renderingAreaRect.getHeight();

    glColor4f(0.0, 0.0, 0.0, opacity);

    glRectf(
            renderingAreaRect.getTopLeftCorner().x,
            renderingAreaRect.getTopLeftCorner().y,
            renderingAreaRect.getBottomRightCorner().x,
            renderingAreaRect.getBottomRightCorner().y
    );

    glBegin(GL_POINTS);
    int x = renderingAreaRect.getTopLeftCorner().x;
    for (double mapX = mapRect.getTopLeftCorner().x; mapX < mapRect.getTopRightCorner().x; mapX += stepX) {
        int y = renderingAreaRect.getTopLeftCorner().y;
        for (double mapY = mapRect.getTopLeftCorner().y; mapY < mapRect.getBottomLeftCorner().y; mapY += stepY) {
            unsigned char value = getValueActual(mapX, mapY);

            if (value != 0) {
                if (value % 3 == 2) glColor4f(0.7, 0.0, 0.0, opacity); // red
                if (value % 3 == 1) glColor4f(0.0, 0.7, 0.0, opacity); // green
                if (value % 3 == 0) glColor4f(0.0, 0.0, 0.7, opacity); // blue

                glVertex2i(x, y);
            }
            y++;
        }
        x++;
    }
    glEnd();

    glColor4f(0.6, 0.6, 1.0, opacity);

    glBegin(GL_LINE_LOOP);
    glVertex2i(renderingAreaRect.getTopLeftCorner().x, renderingAreaRect.getTopLeftCorner().y - 1);
    glVertex2i(renderingAreaRect.getTopRightCorner().x, renderingAreaRect.getTopRightCorner().y - 1);
    glVertex2i(renderingAreaRect.getBottomRightCorner().x + 1, renderingAreaRect.getBottomRightCorner().y);
    glVertex2i(renderingAreaRect.getBottomLeftCorner().x + 1, renderingAreaRect.getBottomLeftCorner().y);
    glEnd();

    glDisable(GL_BLEND);

    glColor3f(1.0, 1.0, 1.0);

    Point center = Point(
            renderingAreaRect.getTopLeftCorner().x + renderingAreaRect.getWidth() / 2,
            renderingAreaRect.getTopLeftCorner().y + renderingAreaRect.getHeight() / 2
    );

    glBegin(GL_POINTS);
    glVertex2i(center.x, center.y);
    glVertex2i((int)center.x - 1, center.y);
    glVertex2i((int)center.x + 1, center.y);
    glVertex2i((int)center.x, (int)center.y - 1);
    glVertex2i((int)center.x, (int)center.y + 1);
    glEnd();

}
