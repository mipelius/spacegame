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

#include "precompile.h"
#include "GameWorldRenderer.h"
#include "GameWorld.h"
#include "GameEntity.h"
#include "Camera.h"

GameWorldRenderer::GameWorldRenderer(Rect const &renderingAreaRect) : RendererBase(renderingAreaRect) {
    gameWorld_ = nullptr;
}


void GameWorldRenderer::setGameWorld(GameWorld *gameWorld) {
    gameWorld_ = gameWorld;
}

void GameWorldRenderer::render() {
    std::list<GameEntity*> *gameEntities = gameWorld_->getGameEntities();

    for (std::list<GameEntity*>::iterator it = gameEntities->begin(); it != gameEntities->end(); it++) {
        Point location = (*it)->getLocation();

        (*it)->render(
                location.x - camera_->getLocation().x,
                location.y - camera_->getLocation().y
        );
    }
}
