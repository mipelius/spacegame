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
                    {"images/anim_rocket_fire.png"}
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
