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

#include "MathUtils.h"
#include "Tile2D.h"
#include "PartialLightMap.h"

GLuint ShadowMask::glShadowTextureId_ = 0;

ShadowMask::ShadowMask() :

ambientLight    (   Property<double>( &ambientLight_ )    ),
ambientLight_   (   1.0 )

{
    if (glShadowTextureId_ == 0) {
        createShadowTexture();
    }
}


void ShadowMask::update(const Canvas& canvas) {
    // turn projection upside down

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, 0, h, -1, 1);

    // draw shadow map to texture

    glColor4d(0, 0, 0, 1.0);
    glRectd(0, 0, w, h);

    drawShadowMap(canvas);

    glBindTexture(GL_TEXTURE_2D, glTextureId_);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, (int)w, (int)h);

    // ------- draw lights -------
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);

    glColor4d(0, 0, 0, 1.0);
    glRectd(0, 0, w, h);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);

    glBlendFunc(GL_DST_ALPHA, GL_ZERO);
    glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);

    glBindTexture(GL_TEXTURE_2D, PointLight::glTextureId_);

    glBegin(GL_QUADS);

    for (auto& dynamicLight : dynamicLights_) {
        dynamicLight->draw(canvas);
    }

    glEnd();

    // blend it with map shadows

    glColor4f(0, 0, 0, 1.0);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, glTextureId_);

    glBegin(GL_QUADS);
    glTexCoord2d(0, 0);
    glVertex2d(0, 0);
    glTexCoord2d(1, 0);
    glVertex2d(w, 0);
    glTexCoord2d(1, 1);
    glVertex2d(w, h);
    glTexCoord2d(0, 1);
    glVertex2d(0, h);
    glEnd();

    glDisable(GL_BLEND);

    // update mask texture

    glBindTexture(GL_TEXTURE_2D, glTextureId_);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, (int)w, (int)h);
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

    // TODO: should get these values from current map
    int blockW = 10; int blockH = 10;

    auto partialLightMap = new PartialLightMap(
            (int)(light->radius_ * 2 / blockW),
            (int)(light->radius_ * 2 / blockH)
    );

    light->partialLightMap_ = partialLightMap;

    dynamicLights_.push_back(light);
}

void ShadowMask::removeLight(PointLight *light) {
    dynamicLights_.remove(light);
    delete light->partialLightMap_;
}

void ShadowMask::init() {
    w = Tile2D::window().w.get();
    h = Tile2D::window().h.get();

    // TODO create dynamicLightScene_ when map is loaded!
    // --- get mapBlockW and mapBlockH from map! ---

    int mapBlockW = 10;
    int mapBlockH = 10;

    dynamicLightScene_ = new Array2d<unsigned char>(
        (int)(w / mapBlockW + 2),
        (int)(h / mapBlockW + 2)
    );

    if (glShadowTextureId_ == 0) {
        createShadowTexture();
    }

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
    TileMap* tileMap = Tile2D::physicsWorld().getMap();

    if (tileMap == nullptr) {
        return;
    }

    Rect rect = canvas.getCamera()->areaRect.get();
    updateDynamicScene(&rect);

    int xStart = 0;
    int xEnd = 0;

    if (rect.x1 > 0) {
        xStart = (int)(rect.x1 / tileMap->getBlockW());
    }

    if (rect.x2 > tileMap->getActualW()) {
        xEnd = tileMap->getW();
    }

    else {
        xEnd = xStart + (int)(rect.getWidth() / tileMap->getBlockW()) + 2;
    }

    int yStart = 0;
    int yEnd = 0;

    if (rect.y1 > 0) {
        yStart = (int)(rect.y1 / tileMap->getBlockH());
    }

    if (rect.y2 > tileMap->getActualW()) {
        yEnd = tileMap->getW();
    }

    else {
        yEnd = yStart + (int)(rect.getHeight() / tileMap->getBlockH()) + 2;
    }

    // NOTE: Texture is disabled for now, since we're going to rewrite the whole rendering later:
    //       We are going to use VBO and shaders to make rendering much faster!

    glDisable(GL_TEXTURE_2D);

    // set blending function

    glEnable(GL_BLEND);
    glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_QUADS);

    int dynX = 0;

    for (int x = xStart; x < xEnd; x++) {
        int dynY = 0;

        for (int y = yStart; y < yEnd; y++) {
            unsigned char lightAmount = dynamicLightScene_->getValue(dynX, dynY);

            float xActual = x * tileMap->getBlockW() - (float)rect.x1;
            float yActual = y * tileMap->getBlockH() - (float)rect.y1;

            float x1 = xActual;
            float y1 = yActual;
            float x2 = xActual + tileMap->getBlockW();
            float y2 = yActual + tileMap->getBlockH();

            auto value = lightAmount / 255.0f;

            if (value > 0) {
                glColor4f(0.0, 0.0, 0.0, value);

                glVertex2f(x1, y1);
                glVertex2f(x2, y1);
                glVertex2f(x2, y2);
                glVertex2f(x1, y2);
            }

            dynY++;
        }
        dynX++;
    }

    glEnd();

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
    auto tileMap = Tile2D::physicsWorld().getMap();

    int cameraX = (int)(areaRect->x1 / tileMap->getBlockW());
    int cameraY = (int)(areaRect->y1 / tileMap->getBlockH());

    int centerX = cameraX + dynamicLightScene_->getW() / 2;
    int centerY = cameraY + dynamicLightScene_->getH() / 2;

    int maxDistance = dynamicLightScene_->getW();

    dynamicLightScene_->clear();

    for (auto dynamicLight : dynamicLights_) {
        PartialLightMap* partialLightMap = dynamicLight->partialLightMap_;

        int partialLightMapCenterX = (int)(dynamicLight->position.get().x / tileMap->getBlockW());
        int partialLightMapCenterY = (int)(dynamicLight->position.get().y / tileMap->getBlockH());

        partialLightMap->setCenterLocation(
                partialLightMapCenterX,
                partialLightMapCenterY
        );

        if (MathUtils::getLength(centerX - partialLightMapCenterX, centerY - partialLightMapCenterY) > maxDistance) {
            continue;
        }

        if (partialLightMap->needsUpdate()) {
            partialLightMap->clear();
            partialLightMap->update(tileMap);
        }

        for (int x = 0; x < partialLightMap->getW(); x++) {
            for (int y = 0; y < partialLightMap->getH(); y++) {
                int dynX = x + partialLightMap->getX() - cameraX;
                int dynY = y + partialLightMap->getY() - cameraY;

                if (dynamicLightScene_->isInsideBounds(dynX, dynY)) {
                    unsigned char newValue = partialLightMap->getValue(x, y);

                    if (dynamicLightScene_->getValue(dynX, dynY) < newValue) {
                        dynamicLightScene_->setValue(dynX, dynY, newValue);
                    }
                }
            }
        }
    }
}
