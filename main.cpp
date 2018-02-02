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

#include "Resources.h"
#include "SceneInGame.h"
#include "SceneTitleScreen.h"
#include "SceneGameOver.h"
#include "Scenes.h"
#include "ColliderLayers.h"

#undef main

int main(int argc, const char *argv[]) {
    Tile2D::load(
            "json/config.json",
            "json/resources.json",

            {
                    {Scenes::titleScreen,    new SceneTitleScreen},
                    {Scenes::inGame,         new SceneInGame},
                    {Scenes::gameOverScreen, new SceneGameOver}
            },

            new ColliderLayerMatrix({
                    {ColliderLayers::playerAmmo, ColliderLayers::playerAmmo, false},
                    {ColliderLayers::playerAmmo, ColliderLayers::enemyAmmo, false},
                    {ColliderLayers::playerAmmo, ColliderLayers::player, false},
                    {ColliderLayers::playerAmmo, ColliderLayers::enemy, true},

                    {ColliderLayers::enemyAmmo, ColliderLayers::enemyAmmo, false},
                    {ColliderLayers::enemyAmmo, ColliderLayers::player, true},
                    {ColliderLayers::enemyAmmo, ColliderLayers::enemy, false},

                    {ColliderLayers::player, ColliderLayers::player, false},
                    {ColliderLayers::player, ColliderLayers::enemy, true},

                    {ColliderLayers::enemy, ColliderLayers::enemy, false}
            })
    );

    return 0;
}
