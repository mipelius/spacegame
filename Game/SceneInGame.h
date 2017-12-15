// This file is part of SpaceGame.
// Copyright (C) 2017 Miika Pelkonen
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

#ifndef __SceneInGame_H
#define __SceneInGame_H

#include "IScene.h"
#include "Vec.h"
#include "EventArgs.h"
#include "Body.h"

class SceneInGame : public IScene {
    void init() override;
    void destroy() override;

    class Body_MapCollisionEventHandler : public IEventHandler<Body, MapCollisionEventArgs> {
        void handle(Body* body, MapCollisionEventArgs args) {
            Vec velNormalized = body->velocity.get().normalized();
            Vec proj_n_v = args.tileNormal * velNormalized.dot(args.tileNormal);
            Vec reflVel = velNormalized - proj_n_v * 2.0;
            body->velocity.set(reflVel * body->velocity.get().length() / 3);
            if (body->velocity.get().lengthSqr() < 1.0) {
                body->velocity.set(Vec(0, 0));
            } else {

                body->position.set(body->position.get() + reflVel);

            }
        }
    };

    Body_MapCollisionEventHandler body_mapCollisionEventHandler;
};


#endif //__SceneInGame_H
