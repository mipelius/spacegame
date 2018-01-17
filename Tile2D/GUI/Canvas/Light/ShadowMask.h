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
#include "Property.h"
#include "Canvas.h"
#include "PointLight.h"
#include "Array2d.h"
#include "Camera.h"
#include "IShadowMask.h"

class PointLight;
class TileMap;
class LightMap;
class PartialLightMapUpdate;

class ShadowMask : public IShadowMask {
    friend class Tile2D;

public:
    ShadowMask();
    ~ShadowMask() = default;

    Property<double> const ambientLight;
    BooleanProperty const softShadowsEnabled;
    BooleanProperty const blendedShadowsEnabled;

    void update(const Canvas& canvas) override;
    void draw(const Canvas& canvas) override;

    void addLight(PointLight *light);
    void removeLight(PointLight *light);

private:
    std::list<PointLight*> lights_;
    std::list<PointLight*> staticLights_;

    Array2d<unsigned char>* lightMap_;

    double ambientLight_;
    double w;
    double h;

    bool softShadowsEnabled_;
    bool blendedShadowsEnabled_;

    void init();

    GLuint glTextureId_;

    static GLuint glShadowTextureId_;

    static const int MAX_PARTIAL_LIGHT_MAP_UPDATE_TIME = 5;

    void drawShadowMap(const Canvas& canvas);

    static void createShadowTexture();
    static const int SHADOW_TEXTURE_SIZE = 8;

    void updateShadowMap(Rect *areaRect);

    void updateInternal(
            unsigned char lastLight,
            int currentX,
            int currentY,
            const int& centerX,
            const int& centerY,
            const int& offsetX,
            const int& offsetY,
            const int& radius,
            Array2d<unsigned char>* lightMap,
            const TileMap* map
    );
};

#endif //__ShadowMask_H_
