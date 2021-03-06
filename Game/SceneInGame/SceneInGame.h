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


#ifndef __SceneInGame_H
#define __SceneInGame_H

#include "Prefab.h"
#include "Tile2D.h"
#include "IScene.h"
#include "Vec.h"
#include "Body.h"
#include "Camera.h"
#include "TeleportBehaviour.h"

class SceneInGame :
        public IScene,
        public ISerializable
{
public:
    void deserialize(const json::Object &jsonObject) override;

private:
    struct PrefabSetup {
        Prefab* prefab;
        Vecf    position;
    };

    void init() override;
    void destroy() override;

    Camera* camera_ = nullptr;

    std::string mapPath_;
    std::string tileSetPath_;

    Prefab* playerPrefab_;
    Prefab* bossPrefab_;

    std::vector<Prefab*> spawnerPrefabs_;
    std::vector<PrefabSetup> otherPrefabs_;

    GameObject* createTeleport(GameObject *player, const Vecf& position);

};


#endif //__SceneInGame_H
