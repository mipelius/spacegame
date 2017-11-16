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
#include "Tile2D.h"

#undef main

int main(int argc, const char *argv[]) {
    SceneTitleScreen sceneTitleScreen = SceneTitleScreen();
    SceneInGame sceneInGame = SceneInGame();
    SceneGameOver sceneGameOver = SceneGameOver();

    Tile2D::load(
            // CONFIGURATION
            "json/config.json",

            // RESOURCES / TEXTURE
            {
                    {"images/anim_explosion.png"},
                    {"images/anim_rocket_fire.png"},
                    {"images/spaceship.png"},
                    {"images/bg1.jpg"},
                    {"images/bg2.jpg"}
            },

            // RESOURCES / SFX
            {
                    {"soundfx/explosion.wav"},
                    {"soundfx/missile_collision.wav"}
            },

            // RESOURCES / MUSIC
            {
                    {"music/spacegame.mp3"},
                    {"music/testing.mp3"}
            },

            // RESOURCES / FONTS
            {
                    {"json/smallfont.json"},
            },

            // RESOURCES / BLOCKMAPPINGS
            {
                    {"json/blockMappings.json"},
            },

            // SCENES
            {
                    {titleScreen,    &sceneTitleScreen},
                    {inGame,         &sceneInGame},
                    {gameOverScreen, &sceneGameOver}
            }
    );

    return 0;
}
