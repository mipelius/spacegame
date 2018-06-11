// MIT License
//
// This file is part of SpaceGame.
// Copyright (c) 2014-2018 Miika Pelkonen
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


#include "Resources.h"
#include "SceneInGame.h"
#include "SceneTitleScreen.h"
#include "SceneEndScreen.h"
#include "Scenes.h"
#include "ColliderLayers.h"
#include "SceneQuickTesting.h"
#include "ObjectCreator.h"
#include "GameObject.h"

// SERIALIZABLE CLASSES

#include "Body.h"
#include "PolygonCollider.h"
#include "Sprite.h"
// ... etc


#undef main

int main(int argc, const char *argv[]) {
    Tile2D::load(
            "data/config.json",
            "data/resources.json",
            "data/gameSetup/sortingLayers.json",
            {
                    {Scenes::titleScreen,    new SceneTitleScreen},
                    {Scenes::inGame,         new SceneInGame},
                    {Scenes::gameEndScreen,  new SceneEndScreen},
                    {Scenes::quickTesting,   new SceneQuickTesting}
            },
            {
                    {ColliderLayers::playerAmmo, ColliderLayers::playerAmmo, false},
                    {ColliderLayers::playerAmmo, ColliderLayers::enemyAmmo, false},
                    {ColliderLayers::playerAmmo, ColliderLayers::player, false},
                    {ColliderLayers::playerAmmo, ColliderLayers::enemy, true},
                    {ColliderLayers::playerAmmo, ColliderLayers::playerPickup, false},

                    {ColliderLayers::enemyAmmo, ColliderLayers::enemyAmmo, false},
                    {ColliderLayers::enemyAmmo, ColliderLayers::player, true},
                    {ColliderLayers::enemyAmmo, ColliderLayers::enemy, false},
                    {ColliderLayers::enemyAmmo, ColliderLayers::playerPickup, false},

                    {ColliderLayers::player, ColliderLayers::player, false},
                    {ColliderLayers::player, ColliderLayers::enemy, true},
                    {ColliderLayers::player, ColliderLayers::playerPickup, true},

                    {ColliderLayers::enemy, ColliderLayers::enemy, false},
                    {ColliderLayers::enemy, ColliderLayers::playerPickup, false},

                    {ColliderLayers::playerPickup, ColliderLayers::playerPickup, false}
            },
            {
                    { "Body",            new ObjectCreator<Body>()            },
                    { "PolygonCollider", new ObjectCreator<PolygonCollider>() },
                    { "Sprite",          new ObjectCreator<Sprite>()          }
            }
    );

    return 0;
}
