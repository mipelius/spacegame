// This file is part of SpaceGame.
// Copyright (C) 2018 Miika Pelkonen
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

#ifndef __PulseLightBehaviour_H
#define __PulseLightBehaviour_H

#include "PointLight.h"
#include "LimitedLifetimeBehaviour.h"

class PulseLightBehaviour : public LimitedLifetimeBehaviour {
public:
    float getRadiusDiminishSpeed() const;
    void setRadiusDiminishSpeed(float radiusDiminishSpeed);
    float getIntensityDiminishSpeed() const;
    void setIntensityDiminishSpeed(float intensityDiminishSpeed);
    Uint32 getTimeToStartDiminish() const;
    void setTimeToStartDiminish(Uint32 timeToStartDiminish);

protected:
    void awake() override;
    void update() override;

    void lateUpdate() override;

private:
    float radiusDiminishSpeed_ = 1.0f;
    float intensityDiminishSpeed_ = 1.0f;

    PointLight* light = nullptr;
    float radiusOnAwake_ = 0.0f;
    float intensityOnAwake_ = 0.0f;
    Uint32 timeToStartDiminish_ = 0;

};

#endif //__PulseLightBehaviour_H
