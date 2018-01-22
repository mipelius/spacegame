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
class TileMap;
class PartialLightMap;

#include "Event.h"
#include <SDL2/SDL_opengl.h>
#include "Tile2DComponent.h"
#include "Vec.h"

class PointLight : public Tile2DComponent {
    friend class LightSystem;

public:
    virtual void draw(const Canvas &canvas);

    PointLight();
    ~PointLight() = default;

    // getters and setters

    float getRadius() const;
    void setRadius(float radius);
    float getIntensity() const;
    void setIntensity(float intensity);

protected:
    void init() override;
    void onDestroy() override;

private:
    static GLuint glTextureId_;

    float radius_;
    float intensity_;

    static const int TEXTURE_SIZE = 32;
    static void createLightTexture();
};


#endif //__PointLight_H_
