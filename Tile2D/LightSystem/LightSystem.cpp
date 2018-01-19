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

GLuint LightSystem::glShadowTextureId_ = 0;

LightSystem::LightSystem() :

ambientLight            (   Property<double> ( &ambientLight_            )    ),
softShadowsEnabled      (   BooleanProperty  ( &softShadowsEnabled_      )    ),
blendedShadowsEnabled   (   BooleanProperty  ( &blendedShadowsEnabled_   )    ),

ambientLight_           (   1.0  ),
softShadowsEnabled_     (   true ),
blendedShadowsEnabled_  (   true )

{ }

void LightSystem::init() {
    w = Tile2D::window().w.get();
    h = Tile2D::window().h.get();

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


void LightSystem::makeLightMap_() {
    int tileW = Tile2D::tileMap().getTileSet()->getTileW();
    int tileH = Tile2D::tileMap().getTileSet()->getTileH();

    if (lightMap_ != nullptr) {
        delete lightMap_;
    }

    lightMap_ = new Array2d<unsigned char>(
            (int)(w / tileW + 2),
            (int)(h / tileH + 2)
    );
}


void LightSystem::addLight(PointLight *light) {
    if (light == nullptr) {
        return;
    }
    lights_.push_back(light);
}

void LightSystem::removeLight(PointLight *light) {
    lights_.remove(light);
}

// ------------------------ RENDERING --------------------------

void LightSystem::update(const Canvas& canvas) {
    if (canvas.getCamera() == nullptr) {
        return;
    }
    // turn projection upside down

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, 0, h, -1, 1);

    // draw shadow map to texture

    glColor4d(0, 0, 0, 1.0);
    glRectd(0, 0, w, h);

    drawLightMap(canvas);

    glBindTexture(GL_TEXTURE_2D, glTextureId_);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, (int)w, (int)h);

    if (blendedShadowsEnabled_) {
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

        for (auto& dynamicLight : lights_) {
            dynamicLight->draw(canvas);
        }

        glEnd();

        // blend with map shadows

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

        // update mask texture

        glBindTexture(GL_TEXTURE_2D, glTextureId_);
        glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, (int)w, (int)h);
    }
}

void LightSystem::draw(const Canvas& canvas) {
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

void LightSystem::drawLightMap(const Canvas &canvas) {
    TileMap* tileMap = Tile2D::physicsWorld().getMap();

    if (tileMap == nullptr) {
        return;
    }

    Rect rect = canvas.getCamera()->areaRect.get();
    updateLightMap(&rect);

    int xStart = (int)(rect.x1 / tileMap->getTileSet()->getTileW());
    int xEnd = xStart + lightMap_->w;
    int yStart = (int)(rect.y1 / tileMap->getTileSet()->getTileH());;
    int yEnd = yStart + lightMap_->h;

    // set blending function

    glEnable(GL_BLEND);
    glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);

    if (softShadowsEnabled_) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, glShadowTextureId_);

        glBegin(GL_QUADS);

        int dynX = 0;

        for (int x = xStart; x < xEnd; x++) {
            int dynY = 0;

            for (int y = yStart; y < yEnd; y++) {
                unsigned char lightAmount = lightMap_->getValue(dynX, dynY);
                float x1 = x * tileMap->getTileSet()->getTileW() - (float)rect.x1;
                float y1 = y * tileMap->getTileSet()->getTileH() - (float)rect.y1;
                float x2 = x1 + tileMap->getTileSet()->getTileW();
                float y2 = y1 + tileMap->getTileSet()->getTileH();

                if (lightAmount > 0) {
                    auto value = lightAmount / 255.0f;
                    glColor4f(0.0, 0.0, 0.0, value);

                    x1 -= tileMap->getTileSet()->getTileW() * 0.75;
                    y1 -= tileMap->getTileSet()->getTileH() * 0.75;
                    x2 += tileMap->getTileSet()->getTileW() * 0.75;
                    y2 += tileMap->getTileSet()->getTileH() * 0.75;

                    glTexCoord2f(0.0, 0.0);
                    glVertex2f(x1, y1);
                    glTexCoord2f(1.0, 0.0);
                    glVertex2f(x2, y1);
                    glTexCoord2f(1.0, 1.0);
                    glVertex2f(x2, y2);
                    glTexCoord2f(0.0, 1.0);
                    glVertex2f(x1, y2);
                }

                dynY++;
            }
            dynX++;
        }

        glEnd();
    }

    else {
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, glShadowTextureId_);

        glBegin(GL_QUADS);

        int dynX = 0;

        for (int x = xStart; x < xEnd; x++) {
            int dynY = 0;

            for (int y = yStart; y < yEnd; y++) {
                unsigned char lightAmount = lightMap_->getValue(dynX, dynY);

                float x1 = x * tileMap->getTileSet()->getTileW() - (float)rect.x1;
                float y1 = y * tileMap->getTileSet()->getTileH() - (float)rect.y1;
                float x2 = x1 + tileMap->getTileSet()->getTileW();
                float y2 = y1 + tileMap->getTileSet()->getTileH();

                if (lightAmount > 0) {
                    auto value = lightAmount / 255.0f;
                    glColor4f(0.0, 0.0, 0.0, value);

                    glTexCoord2f(0.0, 0.0);
                    glVertex2f(x1, y1);
                    glTexCoord2f(1.0, 0.0);
                    glVertex2f(x2, y1);
                    glTexCoord2f(1.0, 1.0);
                    glVertex2f(x2, y2);
                    glTexCoord2f(0.0, 1.0);
                    glVertex2f(x1, y2);
                }

                dynY++;
            }
            dynX++;
        }

        glEnd();
    }
}

void LightSystem::createShadowTexture() {
    Uint8 alphaValue;
    double temp;

    Uint32* pixels = new Uint32[LIGHT_TEXTURE_SIZE * LIGHT_TEXTURE_SIZE];

    double fullLightThreshold = LIGHT_TEXTURE_SIZE / (2 * sqrt(2));

    for (int i = 0; i < LIGHT_TEXTURE_SIZE; i++) {
        for (int j = 0; j < LIGHT_TEXTURE_SIZE; j++) {
            double deltaX = LIGHT_TEXTURE_SIZE/2 - i;
            double deltaY = LIGHT_TEXTURE_SIZE/2 - j;

            double distance = sqrt(deltaX * deltaX + deltaY * deltaY);

            temp = distance / (LIGHT_TEXTURE_SIZE / 2);
            temp *= 8;
            temp -= 7;

            // clamp

            if (temp < 0.0) {
                temp = 0.0;
            }

            if (temp > 1.0) {
                temp = 1.0;
            }

            alphaValue = (Uint8)(temp * 255);
            pixels[j * LIGHT_TEXTURE_SIZE + i] = (0x000000FF - alphaValue) << 24;
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
    glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, LIGHT_TEXTURE_SIZE, LIGHT_TEXTURE_SIZE, 0,
            texture_format, GL_UNSIGNED_BYTE, pixels);

    delete[] pixels;
}

// --------------------- COMPUTE THE SHADOW MAP -------------------------

void LightSystem::updateLightMap(Rect *areaRect) {
    auto tileMap = Tile2D::physicsWorld().getMap();

    lightMap_->fill(0);

    int offsetX = (int)(areaRect->x1 / tileMap->getTileSet()->getTileW());
    int offsetY = (int)(areaRect->y1 / tileMap->getTileSet()->getTileH());

    for (auto light : lights_) {
        int currentLightCenterX = (int)(light->position.get().x / tileMap->getTileSet()->getTileW());
        int currentLightCenterY = (int)(light->position.get().y / tileMap->getTileSet()->getTileH());
        int radius              = (int)(light->radius.get()     / tileMap->getTileSet()->getTileW());

        // TODO: light source that has its center outside the view will be ignored -> FIX THAT

        updateLightMapRecursive(
                currentLightCenterX,
                currentLightCenterY,
                currentLightCenterX,
                currentLightCenterY,
                offsetX,
                offsetY,
                radius,
                0
        );
    }
}

void LightSystem::updateLightMapRecursive(
        int currentX,
        int currentY,
        const int &centerX,
        const int &centerY,
        const int &offsetX,
        const int &offsetY,
        const int &radius,
        unsigned int encounteredWallness
) {
    int lightMapX = currentX - offsetX;
    int lightMapY = currentY - offsetY;

    if (!lightMap_->isInsideBounds(lightMapX, lightMapY)) { // outside the shadow map
        return;
    }

    int lightX = currentX - centerX;
    int lightY = currentY - centerY;

    int length = MathUtils::getLength(lightX, lightY);
    int distanceToBorder = radius - length;

    if (distanceToBorder < 1) { // light can't spread further
        return;
    }

    double translucency = 1.0;

    Tile* currentBlock = Tile2D::tileMap().getValue(currentX, currentY);
    if (currentBlock != nullptr) {
        translucency = currentBlock->translucency.get();
    }

    encounteredWallness += 256 - translucency * 256.0;

    auto lightBasedOnDistance = (int)(255 * (sqrt((float)distanceToBorder / radius)));

    int newLight = lightBasedOnDistance - encounteredWallness;

    if (newLight <= lightMap_->getValue(lightMapX, lightMapY)) return;
    if (newLight < 0) return;

    lightMap_->setValue(lightMapX, lightMapY, newLight);

    updateLightMapRecursive(currentX - 1, currentY, centerX, centerY, offsetX, offsetY, radius, encounteredWallness);
    updateLightMapRecursive(currentX + 1, currentY, centerX, centerY, offsetX, offsetY, radius, encounteredWallness);
    updateLightMapRecursive(currentX, currentY - 1, centerX, centerY, offsetX, offsetY, radius, encounteredWallness);
    updateLightMapRecursive(currentX, currentY + 1, centerX, centerY, offsetX, offsetY, radius, encounteredWallness);
}

