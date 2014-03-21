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
#include "IShadowMask.h"
#include "Property.h"

class PointLight;
class Map;
class ShadowMap;

class ShadowMask : public IShadowMask {

    friend class ShadowMap;

public:
    ShadowMask(double w, double h, Map* map);
    ~ShadowMask();

    Property<double>* const ambientLight;

    void update(Canvas* canvas);
    void draw(Canvas* canvas);

    void addStaticLight(PointLight* light);
    void addDynamicLight(PointLight* light);

private:
    std::list<PointLight*> dynamicLights_;
    std::list<PointLight*> staticLights_;

    double ambientLight_;
    double w_;
    double h_;

    void initialize();

    GLuint glTextureId_;

    ShadowMap* shadowMap_;
};


#endif //__ShadowMask_H_
