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


#include "BombBehaviour.h"
#include "Tile2D.h"

void BombBehaviour::awake() {
    body_ = gameObject()->getComponent<Body>();
    body_->mapCollision.add(&body_mapCollisionEventHandler);
}

void BombBehaviour::update() {

}

void BombBehaviour::lateUpdate() {
    transform()->setRotation(body_->getVelocity().angle());
}

void BombBehaviour::Body_MapCollisionEventHandler::handle(Body *body, MapCollisionEventArgs args) {
    for (auto x=-explosionRadius; x<explosionRadius; ++x) {
        for (auto y=-explosionRadius; y<explosionRadius; ++y) {
            if (Vecf((float)x, (float)y).length() > explosionRadius) {
                continue;
            }
            Vecf offset = {
                    (float)(x * Tile2D::tileMap().getTileSet()->getTileW()),
                    (float)(y * Tile2D::tileMap().getTileSet()->getTileH())
            };
            Tile2D::tileMap().setValueScaled(
                    args.tileCoordinates + offset,
                    Tile2D::tileMap().getTileSet()->getEmptyBlock()
            );
        }
    }

    body->gameObject()->getComponent<Sprite>()->setIsVisible(false);
    body->gameObject()->destroy();
}