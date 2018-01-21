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

private:
    std::vector<PointLight*> lights_;
    Array2d<unsigned char>* lightMap_ = nullptr;

    void createLightMap_();

    float ambientLight_;
    float w;
    float h;

    bool softShadowsEnabled_;
    bool blendedShadowsEnabled_;

    void init();

    GLuint glTextureId_;
    static GLuint glShadowTextureId_;

    void drawLightMap(const Canvas &canvas);

    static void createShadowTexture();
    static const int LIGHT_TEXTURE_SIZE = 8;
    static const int MAX_LIGHT_RADIUS = 100; // measured in tiles

    void updateLightMap(Rect *areaRect);

    void updateLightMapRecursive(
            int currentX,
            int currentY,
            const int &centerX,
            const int &centerY,
            const int &offsetX,
            const int &offsetY,
            const int &radius,
            unsigned int encounteredWallness
    );

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
