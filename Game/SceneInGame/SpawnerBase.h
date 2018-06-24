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

#ifndef SPACEGAME_SPAWNERBASE_H
#define SPACEGAME_SPAWNERBASE_H

#include "Tile2DBehaviour.h"
#include "CountDownTimer.h"
#include "Rect.h"

class SpawnerBase : public Tile2DBehaviour {

public:
    GameObject *getPlayer() const;
    void setPlayer(GameObject *player);
    void remove(GameObject *gameObject);
    void setPrefab(const std::string& prefabString);
    unsigned int getMaxSpawnedObjects() const;
    void setMaxSpawnedObjects(unsigned int maxSpawnedObjects);
    void setSpawningDelay(Uint32 milliseconds);
    const Rect &getAreaRect() const;
    void setAreaRect(const Rect &areaRect);

protected:
    void onDestroy() override;
    unsigned int maxSpawnedObjects_ = 5;
    GameObject* player_;
    CountDownTimer timer_ = {};
    Rect areaRect_ = {};

    GameObject* spawn();

private:
    std::string prefabString_;
    std::list<GameObject*> spawnedGameObjects_;

};


#endif //SPACEGAME_SPAWNERBASE_H
