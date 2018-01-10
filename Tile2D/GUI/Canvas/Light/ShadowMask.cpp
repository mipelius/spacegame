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

#include "Tile2D.h"

GLuint ShadowMask::glShadowTextureId_ = 0;

ShadowMask::ShadowMask() :

ambientLight    (   Property<double>( &ambientLight_ )    ),
ambientLight_   (   1.0 )

{
    if (glShadowTextureId_ == 0) {
        createShadowTexture();
    }
    w = Tile2D::window().w.get();
    h = Tile2D::window().h.get();

    initialize();
}


void ShadowMask::update(const Canvas& canvas) {
    // turn projection upside down

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, 0, h, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // draw shadow map to texture

    glColor4d(0, 0, 0, 1.0);
    glRectd(0, 0, w, h);

    // TODO
    // ---> drawShadowMap(canvas); ---> THIS IS THE DIFFICULT PART! :)

//    glBindTexture(GL_TEXTURE_2D, glTextureId_);
//    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, (int)w, (int)h);

    // draw lights

    glColor4d(0, 0, 0, 0.0); // 1.0
    glRectd(0, 0, w, h);

    glEnable(GL_BLEND);
    glBlendFunc(GL_DST_ALPHA, GL_ZERO);
    glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, PointLight::glTextureId_);

    glBegin(GL_QUADS);

//    for (auto& staticLight : staticLights_) {
//        staticLight->draw(canvas);
//    }

    for (auto& dynamicLight : dynamicLights_) {
        dynamicLight->draw(canvas);
    }

    glEnd();

    glDisable(GL_BLEND);

    // blend it with map shadows
//
//    glEnable(GL_BLEND);
//
//    glColor4f(0, 0, 0, 1.0);
//    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
//
//    glBindTexture(GL_TEXTURE_2D, glTextureId_);
//
//    glBegin(GL_QUADS);
//    glTexCoord2d(0, 0);
//    glVertex2d(0, 0);
//    glTexCoord2d(1, 0);
//    glVertex2d(w, 0);
//    glTexCoord2d(1, 1);
//    glVertex2d(w, h);
//    glTexCoord2d(0, 1);
//    glVertex2d(0, h);
//    glEnd();
//
//    glDisable(GL_BLEND);

    // update mask texture

    glDisable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, glTextureId_);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, (int)w, (int)h);
    glBindTexture(GL_TEXTURE_2D, NULL);

    // set "normal" projection

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, h, 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
}

void ShadowMask::draw(const Canvas& canvas) {
    if (canvas.getCamera() == nullptr) {
        return;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, glTextureId_);

    glColor4d(1, 1, 1, 1.0 - ambientLight_);

    Rect rect = canvas.getCamera()->areaRect.get();

    glBegin(GL_QUADS);
    glTexCoord2d(0, 0);
    glVertex2d(rect.x1, rect.y1);
    glTexCoord2d(1, 0);
    glVertex2d(rect.x2, rect.y1);
    glTexCoord2d(1, 1);
    glVertex2d(rect.x2, rect.y2);
    glTexCoord2d(0, 1);
    glVertex2d(rect.x1, rect.y2);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    glDisable(GL_BLEND);
}

void ShadowMask::addLight(PointLight *light) {
    if (light == nullptr) {
        return;
    }

    dynamicLights_.push_back(light);
}

void ShadowMask::removeLight(PointLight *light) {
    dynamicLights_.remove(light);
}

void ShadowMask::initialize() {
    GLenum texture_format = GL_BGRA;
    GLint nOfColors = 4;

    auto pixels = new Uint32[(int)w * (int)h];

    // Have OpenGL generate a texture object handle for us
    glGenTextures(1, &glTextureId_);

    // Bind the texture object
    glBindTexture(GL_TEXTURE_2D, glTextureId_);

    // Set the texture's stretching properties
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Edit the texture object's image data using the information SDL_Surface gives us
    glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, (int)w, (int)h, 0,
            texture_format, GL_UNSIGNED_BYTE, pixels);

    delete[] pixels;
}

void ShadowMask::drawShadowMap(const Canvas& canvas) {
//    Rect rect = canvas.getCamera().areaRect.get();
//    updateDynamicScene(&rect);
//
//    int xStart = 0;
//    int xEnd = 0;
//
//    if (rect.x1 > 0) {
//        xStart = (int)(rect.x1 / tileMap_->getBlockW());
//    }
//
//    if (rect.x2 > tileMap_->getActualW()) {
//        xEnd = tileMap_->getW();
//    }
//    else {
//        xEnd = xStart + (int)(rect.getWidth() / tileMap_->getBlockW()) + 2;
//    }
//
//    int yStart = 0;
//    int yEnd = 0;
//
//    if (rect.y1 > 0) {
//        yStart = (int)(rect.y1 / tileMap_->getBlockH());
//    }
//
//    if (rect.y2 > tileMap_->getActualW()) {
//        yEnd = tileMap_->getW();
//    }
//    else {
//        yEnd = yStart + (int)(rect.getHeight() / tileMap_->getBlockH()) + 2;
//    }
//
//    // bind shadowTexture
//
//    glEnable(GL_TEXTURE_2D);
//    glBindTexture(GL_TEXTURE_2D, glShadowTextureId_);
//
//    // set blending function
//
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);
//
//    float padding = 1.5;
//
//    glBegin(GL_QUADS);
//
//    int dynX = 0;
//
//    for (int x = xStart; x < xEnd; x++) {
//        int dynY = 0;
//
//        for (int y = yStart; y < yEnd; y++) {
//            unsigned char lightAmount = lightMap_->getLightAmount(x, y);
//            unsigned char dynamicLightAmount = dynamicLightScene_->getValue(dynX, dynY);
//
//            if (dynamicLightAmount > lightAmount) {
//                lightAmount = dynamicLightAmount;
//            }
//
//            float xActual = x * tileMap_->getBlockW() - (float)rect.x1;
//            float yActual = y * tileMap_->getBlockH() - (float)rect.y1;
//
//            float x1 = xActual;
//            float y1 = yActual;
//            float x2 = xActual + tileMap_->getBlockW();
//            float y2 = yActual + tileMap_->getBlockH();
//
//            if (lightAmount == 255) {
//                glColor4f(0, 0, 0, 1.0);
//
//                float texMargin = 0.4999;
//
//                glTexCoord2f(0.0f + texMargin, 0.0f + texMargin); glVertex2f(x1, y1);
//                glTexCoord2f(1.0f - texMargin, 0.0f + texMargin); glVertex2f(x2, y1);
//                glTexCoord2f(1.0f - texMargin, 1.0f - texMargin); glVertex2f(x2, y2);
//                glTexCoord2f(0.0f + texMargin, 1.0f - texMargin); glVertex2f(x1, y2);
//            }
//            else if (lightAmount > 0) {
//                auto value = (float)(1.0 - pow(1.0 - lightAmount / 255.0, 0.25));
//
//                glColor4f(0.0, 0.0, 0.0, value);
//
//                x1 -= padding * tileMap_->getBlockW();
//                y1 -= padding * tileMap_->getBlockH();
//                x2 += padding * tileMap_->getBlockW();
//                y2 += padding * tileMap_->getBlockH();
//
//                glTexCoord2f(0.0, 0.0); glVertex2f(x1, y1);
//                glTexCoord2f(1.0, 0.0); glVertex2f(x2, y1);
//                glTexCoord2f(1.0, 1.0); glVertex2f(x2, y2);
//                glTexCoord2f(0.0, 1.0); glVertex2f(x1, y2);
//
//            }
//            dynY++;
//        }
//        dynX++;
//    }
//
//    glEnd();
//
//    glDisable(GL_BLEND);
//    glDisable(GL_TEXTURE_2D);

}

void ShadowMask::createShadowTexture() {
    Uint8 alphaValue;
    double temp;

    Uint32* pixels = new Uint32[SHADOW_TEXTURE_SIZE * SHADOW_TEXTURE_SIZE];

    for (int i = 0; i < SHADOW_TEXTURE_SIZE; i++) {
        for (int j = 0; j < SHADOW_TEXTURE_SIZE; j++) {
            double deltaX = SHADOW_TEXTURE_SIZE/2 - i;
            double deltaY = SHADOW_TEXTURE_SIZE/2 - j;

            double distance = sqrt(deltaX * deltaX + deltaY * deltaY);

            temp = distance / (SHADOW_TEXTURE_SIZE / 2);
            if (temp > 1.0) {
                temp = 1.0;
            }

            alphaValue = (Uint8)(temp * 255);
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

void ShadowMask::updateDynamicScene(Rect *areaRect) {
//    int cameraX = (int)(areaRect->x1 / tileMap_->getBlockW());
//    int cameraY = (int)(areaRect->y1 / tileMap_->getBlockH());
//
//    dynamicLightScene_->clear();
//
//    for (auto& dynamicLight : dynamicLights_) {
//        PartialLightMap* partialLightMap = dynamicLight->partialLightMap_;
//
//        for (int x = 0; x < partialLightMap->getW(); x++) {
//            for (int y = 0; y < partialLightMap->getH(); y++) {
//                int dynX = x + partialLightMap->getX() - cameraX;
//                int dynY = y + partialLightMap->getY() - cameraY;
//
//                if (dynamicLightScene_->isInsideBounds(dynX, dynY)) {
//                    unsigned char newValue = partialLightMap->getValue(x, y);
//
//                    if (dynamicLightScene_->getValue(dynX, dynY) < newValue) {
//                        dynamicLightScene_->setValue(dynX, dynY, newValue);
//                    }
//                }
//            }
//        }
//    }
}
