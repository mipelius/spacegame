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


#ifndef __PointLight_H_
#define __PointLight_H_

class Canvas;
class LightMap;
class TileMap;
class PartialLightMap;

#include "Event.h"
#include "precompile.h"
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
    Tile2DComponent* clone() override;

private:
    static GLuint glTextureId_;

    float radius_;
    float intensity_;

    static const int TEXTURE_SIZE = 32;
    static void createLightTexture();
};


#endif //__PointLight_H_
