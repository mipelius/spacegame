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


#ifndef __PulseLightBehaviour_H
#define __PulseLightBehaviour_H

#include "PointLight.h"
#include "LimitedLifetimeBehaviour.h"

class PulseLightBehaviour :
        public LimitedLifetimeBehaviour
{
public:
    float getRadiusDiminishSpeed() const;
    void setRadiusDiminishSpeed(float radiusDiminishSpeed);
    float getIntensityDiminishSpeed() const;
    void setIntensityDiminishSpeed(float intensityDiminishSpeed);
    Uint32 getTimeToStartDiminish() const;
    void setTimeToStartDiminish(Uint32 timeToStartDiminish);

    void deserialize(const json::Object &jsonObject) override;

protected:
    void awake() override;
    void update() override;

    void lateUpdate() override;

    Tile2DComponent *clone() override;

private:
    float radiusDiminishSpeed_ = 1.0f;
    float intensityDiminishSpeed_ = 1.0f;

    PointLight* light = nullptr;
    float radiusOnAwake_ = 0.0f;
    float intensityOnAwake_ = 0.0f;
    Uint32 timeToStartDiminish_ = 0;

};

#endif //__PulseLightBehaviour_H
