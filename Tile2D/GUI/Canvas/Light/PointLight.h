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

#ifndef __PointLight_H_
#define __PointLight_H_

class Canvas;
class LightMap;
class WorldMap;
class PartialLightMap;

#include "Event.h"
#include <SDL2/SDL_opengl.h>
#include "Property.h"
#include "Point.h"
#include "PointLightMovedEventArgs.h"

class PointLight {
    friend class ShadowMask;
    friend class LocationProperty;

public:
    virtual void draw(Canvas *canvas);

    PointLight(Point location, double radius, bool isDynamic = false);
    ~PointLight();

    Property<Point>* const location;
    Property<double>* const radius;
    Property<double>* const intensity;

    Event<PointLight, PointLightMovedEventArgs>* const movement;

    static GLuint glTextureId_;

private:
    Point location_;
    double radius_;
    double intensity_;

    bool isDynamic_;

    PartialLightMap* partialLightMap_;

    static const int TEXTURE_SIZE = 128;
    static void createLightTexture();

    class LocationProperty;
};


#endif //__PointLight_H_
