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


#include <stack>
#include "Tile2D.h"
#include "Tile2DMath.h"
#include "LightSystem.h"
#include "Window.h"
#include "TileMap.h"
#include "TileSet.h"

GLuint LightSystem::glShadowTextureId_ = 0;

LightSystem::LightSystem() :

ambientLight_           (   1.0  ),
softShadowsEnabled_     (   true ),
blendedShadowsEnabled_  (   true ),
enabled_                (   true )

{
    for (auto x = 0u; x < MAX_LIGHT_RADIUS; ++x) {
        for (auto y = 0u; y < MAX_LIGHT_RADIUS; ++y) {
            intDistances[x][y] = (unsigned int)sqrt(x * x + y * y);
        }
    }
}

void LightSystem::init() {
    auto windowSize = Tile2D::window().getSize();

    if (glShadowTextureId_ == 0) {
        createShadowTexture();
    }

    GLenum texture_format = GL_BGRA;
    GLint nOfColors = 4;

    auto pixels = new Uint32[windowSize.x * windowSize.y];

    // Have OpenGL generate a texture object handle for us
    glGenTextures(1, &glTextureId_);

    // Bind the texture object
    glBindTexture(GL_TEXTURE_2D, glTextureId_);

    // Set the texture's stretching properties
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, windowSize.x, windowSize.y, 0,
                 texture_format, GL_UNSIGNED_BYTE, pixels);

    delete[] pixels;
}


void LightSystem::createLightMap_() {
    int tileW = Tile2D::tileMap().getTileSet()->getTileW();
    int tileH = Tile2D::tileMap().getTileSet()->getTileH();

    if (lightMap_ != nullptr) {
        delete lightMap_;
    }

    auto windowSize = Tile2D::window().getSize();

    lightMap_ = new Array2d<unsigned char>(
            (2 * MAX_LIGHT_RADIUS + windowSize.x / tileW + 2),
            (2 * MAX_LIGHT_RADIUS + windowSize.y / tileH + 2)
    );
}


void LightSystem::addLight(PointLight *light) {
    if (light == nullptr) {
        return;
    }
    lights_.push_back(light);
}

void LightSystem::removeLight(PointLight *light) {
    for (std::vector<PointLight*>::iterator it = lights_.begin(); it != lights_.end(); it++) {
        if (*it == light) {
            lights_.erase(it);
            break;
        }
    }
}

// ------------------------ RENDERING --------------------------

void LightSystem::update(const Canvas& canvas) {
    if (canvas.getCamera() == nullptr || !enabled_ || !Tile2D::tileMap().isLoaded()) {
        return;
    }
    auto windowSize = Tile2D::window().getSize();

    // turn projection upside down
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, windowSize.x, 0, windowSize.y, -1, 1);

    // draw shadow map to texture

    glDisable(GL_TEXTURE_2D);
    glColor4d(0, 0, 0, 1.0);
    glRectd(0, 0, windowSize.x, windowSize.y);

    drawLightMap(canvas);

    glBindTexture(GL_TEXTURE_2D, glTextureId_);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, windowSize.x, windowSize.y);

    if (blendedShadowsEnabled_) {
        // ------- draw lights -------

        glDisable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);

        glColor4d(0, 0, 0, 1.0);
        glRectd(0, 0, windowSize.x, windowSize.y);

        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);

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
        glTexCoord2f(0, 0);
        glVertex2f(0, 0);
        glTexCoord2f(1, 0);
        glVertex2f(windowSize.x, 0);
        glTexCoord2f(1, 1);
        glVertex2f(windowSize.x, windowSize.y);
        glTexCoord2f(0, 1);
        glVertex2f(0, windowSize.y);
        glEnd();

        // update mask texture

        glBindTexture(GL_TEXTURE_2D, glTextureId_);
        glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, windowSize.x, windowSize.y);
    }
}

void LightSystem::draw(const Canvas& canvas) {
    if (canvas.getCamera() == nullptr || !enabled_ || !Tile2D::tileMap().isLoaded()) {
        return;
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, glTextureId_);

    glColor4d(1, 1, 1, 1.0 - ambientLight_);

    Rect rect = canvas.getCamera()->getAreaRect();

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2f(rect.x1, rect.y1);
    glTexCoord2f(1, 0);
    glVertex2f(rect.x2, rect.y1);
    glTexCoord2f(1, 1);
    glVertex2f(rect.x2, rect.y2);
    glTexCoord2f(0, 1);
    glVertex2f(rect.x1, rect.y2);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    glDisable(GL_BLEND);
}

void LightSystem::drawLightMap(const Canvas &canvas) {
    TileMap* tileMap = &Tile2D::tileMap();

    if (tileMap == nullptr) {
        return;
    }

    Rect rect = canvas.getCamera()->getAreaRect();
	updateLightMap(&rect);

    int xStart = (int)(rect.x1 / tileMap->getTileSet()->getTileW());
    int xEnd = xStart + lightMap_->w - MAX_LIGHT_RADIUS;
    int yStart = (int)(rect.y1 / tileMap->getTileSet()->getTileH());;
    int yEnd = yStart + lightMap_->h - MAX_LIGHT_RADIUS;

    // set blending function

    glEnable(GL_BLEND);
    glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);

    if (softShadowsEnabled_) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, glShadowTextureId_);

        glBegin(GL_QUADS);

        int dynX = MAX_LIGHT_RADIUS;

        for (int x = xStart; x < xEnd; x++) {
            int dynY = MAX_LIGHT_RADIUS;

            for (int y = yStart; y < yEnd; y++) {
                unsigned char lightAmount = lightMap_->getValue(dynX, dynY);
                float x1 = x * tileMap->getTileSet()->getTileW() - rect.x1;
                float y1 = y * tileMap->getTileSet()->getTileH() - rect.y1;
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

        int dynX = MAX_LIGHT_RADIUS;

        for (int x = xStart; x < xEnd; x++) {
            int dynY = MAX_LIGHT_RADIUS;

            for (int y = yStart; y < yEnd; y++) {
                unsigned char lightAmount = lightMap_->getValue(dynX, dynY);

                float x1 = x * tileMap->getTileSet()->getTileW() - rect.x1;
                float y1 = y * tileMap->getTileSet()->getTileH() - rect.y1;
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
    float temp;

    Uint32* pixels = new Uint32[LIGHT_TEXTURE_SIZE * LIGHT_TEXTURE_SIZE];

    for (int i = 0; i < LIGHT_TEXTURE_SIZE; i++) {
        for (int j = 0; j < LIGHT_TEXTURE_SIZE; j++) {
            float deltaX = LIGHT_TEXTURE_SIZE/2 - i;
            float deltaY = LIGHT_TEXTURE_SIZE/2 - j;

            float distance = sqrt(deltaX * deltaX + deltaY * deltaY);

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

bool wayToSort(PointLight* a, PointLight* b) { return a->getRadius() > b->getRadius(); }

void LightSystem::updateLightMap(Rect *areaRect) {
    std::sort(lights_.begin(), lights_.end(), wayToSort);

    auto tileMap = &Tile2D::tileMap();

    lightMap_->fill(0);

    Veci offset = {
            (int)(areaRect->x1 / tileMap->getTileSet()->getTileW()) - MAX_LIGHT_RADIUS,
            (int)(areaRect->y1 / tileMap->getTileSet()->getTileH()) - MAX_LIGHT_RADIUS
    };

    // temporary variables used by algorithm
    Veci lightMapPoint                  = {0, 0};
    Veci currentInLightCoordinateSystem = {0, 0};
    int brightnessBasedOnDistance       = 0;
    int tmpWallness                     = 0;
    int tmpBrightness                   = 0;
    unsigned char encounteredWallness   = 0;
    unsigned char brightness            = 0;

    struct Node {
        Veci            coords;
        unsigned char   encounteredWallness;
    };

    for (auto light : lights_) {
        Veci lightOrigin        = {
                (int)(light->transform()->getPosition().x / tileMap->getTileSet()->getTileW()),
                (int)(light->transform()->getPosition().y / tileMap->getTileSet()->getTileH())
        };
        int radius              = (int)(light->getRadius() / tileMap->getTileSet()->getTileW());

        std::stack<Node> nodes;
        nodes.push({lightOrigin, 0});

        while (!nodes.empty()) {
            const auto current = nodes.top();
            nodes.pop();

            // early-out-check

            lightMapPoint = current.coords - offset;
            if (!lightMap_->isInsideBounds(lightMapPoint.x, lightMapPoint.y)) { // outside the light map
                continue;
            }

            currentInLightCoordinateSystem  = current.coords - lightOrigin;
            int length = getLength(currentInLightCoordinateSystem.x, currentInLightCoordinateSystem.y);
            int distanceToBorder = radius - length;

            if (distanceToBorder < 1) { // light can't spread further
                continue;
            }

            // calculate new encountered wallness

            float translucency = 1.0;

            Tile* currentTile = Tile2D::tileMap().getValue(current.coords.x, current.coords.y);
            if (currentTile != nullptr) {
                translucency = currentTile->getTranslucency();
            }

            tmpWallness = current.encounteredWallness + (int)(256.0 - translucency * 256.0);

            // calculate new brightness

            brightnessBasedOnDistance = (int)(255 * ((float)distanceToBorder / radius));
            tmpBrightness = brightnessBasedOnDistance - tmpWallness;

            // check if new brightness can't improve the brightness of the current light map cell

            if (tmpBrightness <= lightMap_->getValue(lightMapPoint.x, lightMapPoint.y)) continue;
            if (tmpBrightness < 0) continue;

            // clamp values to unsigned char

            Mathi::clamp(tmpBrightness, 0, 255);
            Mathi::clamp(tmpWallness, 0, 255);
            brightness           = (unsigned char)tmpBrightness;
            encounteredWallness  = (unsigned char)tmpWallness;

            // update lightmap and push neighbor nodes to the stack

            lightMap_->setValue(lightMapPoint.x, lightMapPoint.y, brightness);

            nodes.push({{current.coords.x - 1, current.coords.y - 0}, encounteredWallness});  // WEST
            nodes.push({{current.coords.x - 0, current.coords.y + 1}, encounteredWallness});  // SOUTH
            nodes.push({{current.coords.x + 1, current.coords.y + 0}, encounteredWallness});  // EAST
            nodes.push({{current.coords.x + 0, current.coords.y - 1}, encounteredWallness});  // NORTH
        }
    }
}

// getters and setters

float LightSystem::getAmbientLight() const {
    return ambientLight_;
}

void LightSystem::setAmbientLight(float ambientLight) {
    ambientLight_ = ambientLight;
}

bool LightSystem::isSoftShadowsEnabled() const {
    return softShadowsEnabled_;
}

void LightSystem::setSoftShadowsEnabled(bool softShadowsEnabled) {
    softShadowsEnabled_ = softShadowsEnabled;
}

bool LightSystem::isBlendedShadowsEnabled() const {
    return blendedShadowsEnabled_;
}

void LightSystem::setBlendedShadowsEnabled(bool blendedShadowsEnabled) {
    blendedShadowsEnabled_ = blendedShadowsEnabled;
}

bool LightSystem::isEnabled() const {
    return enabled_;
}

void LightSystem::setEnabled(bool enabled) {
    enabled_ = enabled;
}

