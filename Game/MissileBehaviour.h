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

#ifndef __MissileBehaviour_H
#define __MissileBehaviour_H

#include "Sprite.h"
#include "Body.h"
#include "Tile2DBehaviour.h"

class MissileBehaviour : public Tile2DBehaviour {

protected:
    void awake() override;
    void update() override;

    class Body_MapCollisionEventHandler : public IEventHandler<Body, MapCollisionEventArgs> {
        void createSparkle(Vec position, Vec velocity);
        void handle(Body* body, MapCollisionEventArgs args) override;
    };

    Body_MapCollisionEventHandler body_mapCollisionEventHandler;

    Uint32 awakeTimestamp;
    const Uint32 TTL = 1000;
};


#endif //__MissileBehaviour_H
