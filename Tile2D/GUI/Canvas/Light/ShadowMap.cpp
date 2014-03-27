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
#include "ShadowMap.h"
#include "Canvas.h"
#include "WorldMap.h"
#include "Camera.h"
#include "LightMap.h"
#include "PointLight.h"
#include "ShadowMask.h"

ShadowMap::ShadowMap(WorldMap *map, ShadowMask* shadowMask) {
    map_ = map;
    shadowMask_ = shadowMask;

    dynamicSceneW_ = (int)(shadowMask_->w_ / map_->getBlockW()) + 1;
    dynamicSceneH_ = (int)(shadowMask_->h_ / map_->getBlockH()) + 1;
    dynamicScene_ = new double[dynamicSceneW_ * dynamicSceneH_];
    memset(dynamicScene_, 0x0, sizeof(double) * dynamicSceneW_ * dynamicSceneH_);

    staticLightMap_ = new double[map_->getW() * map_->getH()];
    memset(staticLightMap_, 0x0, sizeof(double) * map_->getW() * map_->getH());

    createShadowTexture();
}

ShadowMap::~ShadowMap() {
    delete[] dynamicScene_;
    delete[] staticLightMap_;
}

void ShadowMap::draw(Canvas *canvas) {
    updateDynamicScene(canvas);

    Rect rect = canvas->getCamera()->areaRect->get();

    int xStart = 0;
    int xEnd = 0;

    if (rect.x1 > 0) {
        xStart = (int)(rect.x1 / map_->getBlockW());
    }

    if (rect.x2 > map_->getActualW()) {
        xEnd = map_->getW();
    }
    else {
        xEnd = xStart + (int)(rect.getWidth() / map_->getBlockW()) + 2;
    }

    int yStart = 0;
    int yEnd = 0;

    if (rect.y1 > 0) {
        yStart = (int)(rect.y1 / map_->getBlockH());
    }

    if (rect.y2 > map_->getActualW()) {
        yEnd = map_->getW();
    }
    else {
        yEnd = yStart + (int)(rect.getHeight() / map_->getBlockH()) + 2;
    }

    // bind shadowTexture

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, glShadowTextureId_);

    // set blending function

    glEnable(GL_BLEND);
    glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);

    double padding = 1.5;
    double texMargin = 0.15;

//    glDisable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);

    int dynamicSceneX = 0;

    for (int x = xStart; x < xEnd; x++) {

        int dynamicSceneY = 0;

        for (int y = yStart; y < yEnd; y++) {
            double lightAmount = staticLightMap_[x + y * map_->getW()];

            double dynLightAmount = dynamicScene_[dynamicSceneX + dynamicSceneY * dynamicSceneW_];

            if (lightAmount < dynLightAmount) {
                lightAmount = dynLightAmount;
            }

            double xActual = x * map_->getBlockW() - rect.x1;
            double yActual = y * map_->getBlockH() - rect.y1;

            float x1 = xActual;
            float y1 = yActual;
            float x2 = xActual + map_->getBlockW();
            float y2 = yActual + map_->getBlockH();

            if (lightAmount >= 1.0) {
              glColor4f(0, 0, 0, 1.0);
//
                float texMargin = 0.499;

                glTexCoord2f(0.0 + texMargin, 0.0 + texMargin);
                glVertex2f(x1, y1);
                glTexCoord2f(1.0 - texMargin, 0.0 + texMargin);
                glVertex2f(x2, y1);
                glTexCoord2f(1.0 - texMargin, 1.0 - texMargin);
                glVertex2f(x2, y2);
                glTexCoord2f(0.0 + texMargin, 1.0 - texMargin);
                glVertex2f(x1, y2);

            }
            else if (lightAmount > 0.0) {
                double value = lightAmount;

                if (value <= 1.0 - 0.15) {
                    value = 1.0 - pow(1.0 - lightAmount, 0.2);
                }
                else {
                    value = 1.0;
                }

                glColor4f(0.0, 0.0, 0.0, value);

                x1 -= padding * map_->getBlockW();
                y1 -= padding * map_->getBlockH();
                x2 += padding * map_->getBlockW();
                y2 += padding * map_->getBlockH();

                glTexCoord2f(0.0 + texMargin, 0.0 + texMargin);
                glVertex2f(x1, y1);
                glTexCoord2f(1.0 - texMargin, 0.0 + texMargin);
                glVertex2f(x2, y1);
                glTexCoord2f(1.0 - texMargin, 1.0 - texMargin);
                glVertex2f(x2, y2);
                glTexCoord2f(0.0 + texMargin, 1.0 - texMargin);
                glVertex2f(x1, y2);

            }

            dynamicSceneY++;
        }

        dynamicSceneX++;
    }

    glEnd();

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

void ShadowMap::createShadowTexture() {
    Uint8 alphaValue;
    double temp;

    Uint32* pixels = new Uint32[SHADOW_TEXTURE_SIZE * SHADOW_TEXTURE_SIZE];

    for (int i = 0; i < SHADOW_TEXTURE_SIZE; i++) {
        for (int j = 0; j < SHADOW_TEXTURE_SIZE; j++) {
            temp = sin(i * M_PI / SHADOW_TEXTURE_SIZE) * sin(j * M_PI / SHADOW_TEXTURE_SIZE);
            temp = temp * temp * temp * temp;
            alphaValue = (Uint8)(255 - temp * 255);
            pixels[j * SHADOW_TEXTURE_SIZE + i] = (0x000000FF - alphaValue) << 24;
        }
    }

    GLenum texture_format = GL_BGRA;
    GLint nOfColors = 4;

    // Have OpenGL generate a texture object handle for us
    glGenTextures(1, &glShadowTextureId_);

    // Bind the texture object
    glBindTexture(GL_TEXTURE_2D, glShadowTextureId_);

    // Set the texture's stretching properties
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Edit the texture object's image data
    glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, SHADOW_TEXTURE_SIZE, SHADOW_TEXTURE_SIZE, 0,
            texture_format, GL_UNSIGNED_BYTE, pixels);

    delete[] pixels;
}

void ShadowMap::updateStaticLightMap(PointLight *light) {
    light->lightMap_->clear();

    int offsetX = (int)((light->location_.x - light->radius_) / map_->getBlockW());
    int offsetY = (int)((light->location_.y - light->radius_) / map_->getBlockH());
    light->lightMap_->applyLightCenter(map_, offsetX, offsetY);

    // sum values of lightMap to staticLightMap

    double* src = light->lightMap_->data_;
    int w = light->lightMap_->w_;
    int h = light->lightMap_->h_;

    double* dst = staticLightMap_;

    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            int dstX = x + offsetX;
            int dstY = y + offsetY;

            if (dstX < 0 || dstX >= map_->getW()) {
                continue;
            }
            if (dstY < 0 || dstY >= map_->getH()) {
                continue;
            }

            int dstIndex = dstY * map_->getW() + dstX;
            int srcIndex = y * w + x;

            if (src[srcIndex] < dst[dstIndex]) {
                continue;
            }

            double newValue = src[srcIndex];

            if (newValue > 1.0) {
                newValue = 1.0;
            }

            dst[dstIndex] = newValue;
        }    
    }
}

void ShadowMap::updateDynamicScene(Canvas* canvas) {
    int camX = (int)(canvas->getCamera()->areaRect->get().x1 / map_->getBlockW());
    int camY = (int)(canvas->getCamera()->areaRect->get().y1 / map_->getBlockH());

    std::list<PointLight*>& lights = shadowMask_->dynamicLights_;

    memset(dynamicScene_, 0x0, sizeof(double) * dynamicSceneW_ * dynamicSceneH_);

    for (std::list<PointLight*>::iterator i = lights.begin(); i != lights.end(); i++) {
        (*i)->lightMap_->clear();

        int offsetX = (int)(((*i)->location_.x - (*i)->radius_) / map_->getBlockW());
        int offsetY = (int)(((*i)->location_.y - (*i)->radius_) / map_->getBlockH());
        (*i)->lightMap_->applyLightCenter(map_, offsetX, offsetY);

        int w = (*i)->lightMap_->w_;
        int h = (*i)->lightMap_->h_;

        double* src = (*i)->lightMap_->data_;
        double* dst = dynamicScene_;

        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int dstX = x + offsetX - camX;
                int dstY = y + offsetY - camY;

                if (dstX < 0 || dstX >= dynamicSceneW_) {
                    continue;
                }
                if (dstY < 0 || dstY >= dynamicSceneH_) {
                    continue;
                }

                int dstIndex = dstY * dynamicSceneW_ + dstX;
                int srcIndex = y * w + x;

                if (src[srcIndex] < dst[dstIndex]) {
                    continue;
                }

                double newValue = src[srcIndex];

                if (newValue > 1.0) {
                    newValue = 1.0;
                }

                dst[dstIndex] = newValue;
            }
        }

    }
}