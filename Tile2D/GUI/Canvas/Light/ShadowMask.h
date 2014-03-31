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
#include "IShadowMask.h"
#include "Property.h"
#include "Canvas.h"

class PointLight;
class WorldMap;
class LightMap;
class PartialLightMapUpdate;

class ShadowMask : public IShadowMask {

    friend class WorldMap_ModificationEventHandler;
    friend class PointLight_MovementEventHandler;

public:
    ShadowMask(double w, double h, WorldMap * map);
    ~ShadowMask();

    Property<double>* const ambientLight;

    void update(Canvas* canvas);
    void draw(Canvas* canvas);

    void addLight(PointLight *light);

private:
    std::list<PointLight*> dynamicLights_;
    std::list<PointLight*> staticLights_;

    double ambientLight_;
    double w_;
    double h_;

    WorldMap* worldMap_;
    LightMap* lightMap_;

    void initialize();

    void handleNextUpdate();

    std::queue<PartialLightMapUpdate*> partialLightMapUpdatesQueue;

    GLuint glTextureId_;

    static GLuint glShadowTextureId_;

    static const int MAX_PARTIAL_LIGHT_MAP_UPDATE_TIME = 5;

    void drawShadowMap(Canvas *canvas);

    static void createShadowTexture();
    static const int SHADOW_TEXTURE_SIZE = 32;

    class WorldMap_ModificationEventHandler;
    class PointLight_MovementEventHandler;
};


#endif //__ShadowMask_H_
