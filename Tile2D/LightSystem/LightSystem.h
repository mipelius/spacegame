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


#ifndef __ShadowMask_H_
#define __ShadowMask_H_

#include <list>
#include <queue>
#include "Canvas.h"
#include "PointLight.h"
#include "Array2d.h"
#include "Camera.h"

class PointLight;
class LightMap;
class PartialLightMapUpdate;

class LightSystem {
    friend class Tile2D;
    friend class TileMap;

public:
    LightSystem();
    ~LightSystem() = default;

    void update(const Canvas& canvas);
    void draw(const Canvas& canvas);

    void addLight(PointLight *light);
    void removeLight(PointLight *light);

    // getters and setters

    float getAmbientLight() const;
    void setAmbientLight(float ambientLight);
    bool isSoftShadowsEnabled() const;
    void setSoftShadowsEnabled(bool softShadowsEnabled);
    bool isBlendedShadowsEnabled() const;
    void setBlendedShadowsEnabled(bool blendedShadowsEnabled);
    bool isEnabled() const;
    void setEnabled(bool enabled);

private:
    std::vector<PointLight*> lights_;
    Array2d<unsigned char>* lightMap_ = nullptr;

    void createLightMap_();

    float ambientLight_;
    float w;
    float h;

    bool softShadowsEnabled_;
    bool blendedShadowsEnabled_;
    bool enabled_;

private:

    void init();

    GLuint glTextureId_;
    static GLuint glShadowTextureId_;

    void drawLightMap(const Canvas &canvas);

    static void createShadowTexture();
    static const int LIGHT_TEXTURE_SIZE = 8;
    static const int MAX_LIGHT_RADIUS = 100; // measured in tiles

    void updateLightMap(Rect *areaRect);

    unsigned int intDistances[MAX_LIGHT_RADIUS][MAX_LIGHT_RADIUS];

    inline unsigned int getLength(int x, int y) {
        if (x < 0) {
            x = -x;
        }
        if (y < 0) {
            y = -y;
        }

        if (x >= MAX_LIGHT_RADIUS || y >= MAX_LIGHT_RADIUS) {
            return INT_MAX;
        }

        return intDistances[x][y];
    };
};

#endif //__ShadowMask_H_
