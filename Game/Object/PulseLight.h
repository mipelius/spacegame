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


#ifndef __PulseLight_H_
#define __PulseLight_H_

class PointLight;

#include "AnimationBase.h"
#include "PointLight.h"
#include "Point.h"

class PulseLight {
    friend class Animation;

public:
    PulseLight(Point point, double radius);

private:
    class Animation : public AnimationBase {
    private:
        PulseLight* light_;

    protected:
        void updateActual() override;

    public:
        explicit Animation(PulseLight *light_);
        void stop() override;
    };

    Animation animation_;
    PointLight pointLight_;

    double initialRadius_;
};

#endif //__PulseLight_H_
