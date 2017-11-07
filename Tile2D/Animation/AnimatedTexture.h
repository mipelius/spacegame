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

#ifndef __AnimatedTexture_H_
#define __AnimatedTexture_H_

#include "Texture.h"
#include "ITexture.h"
#include "AnimationBase.h"

class AnimatedTexture : public AnimationBase, public ITexture {
private:
    Texture* texture_;

public:
    AnimatedTexture(unsigned int framesPerSecond, unsigned int frameAmount, bool enableLoop, Texture* texture);
    void updateActual() override;

    void glBind() override ;
    void glTexCorner(Corner corner) override;
    void glUnbind() override;

    Event<AnimatedTexture, EventArgs> const stopped;

    void stop() override ;
};

#endif //__AnimatedTexture_H_
