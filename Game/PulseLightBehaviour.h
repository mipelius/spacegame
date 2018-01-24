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
#include "LimitedAgeBehaviour.h"

class PulseLightBehaviour : public LimitedAgeBehaviour {

protected:
    void awake() override;
    void update() override;

    void lateUpdate() override;

    PointLight* light = nullptr;
    float radiusOnAwake_ = 0.0f;
    float intensityOnAwake_ = 0.0f;
    float timeToStartDiminish_ = 0.0f;
public:
    float getTimeToStartDiminish() const;

    void setTimeToStartDiminish(float timeToStartDiminish);

private:
    float radiusDiminishSpeed_ = 1.0f;
    float intensityDiminishSpeed_ = 1.0f;
public:
    float getRadiusDiminishSpeed() const;
    void setRadiusDiminishSpeed(float radiusDiminishSpeed);
    float getIntensityDiminishSpeed() const;
    void setIntensityDiminishSpeed(float intensityDiminishSpeed);


};

#endif //__PulseLightBehaviour_H
