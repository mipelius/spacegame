//
// Created by Miika Pelkonen on 29/06/2018.
//

#ifndef SPACEGAME_EXPLOSIONBEHAVIOUR_H
#define SPACEGAME_EXPLOSIONBEHAVIOUR_H

#include "ISerializable.h"
#include "Tile2DBehaviour.h"

class ExplosionBehaviour :
        public Tile2DBehaviour,
        public ISerializable
{
public:
    void deserialize(const json::Object &jsonObject) override;

    int getRadius_() const;
    void setRadius_(int radius_);

protected:
    Tile2DComponent *clone() override;

    void awake() override;
    void update() override;
    void lateUpdate() override;

private:
    int radius_ = 10;
};

#endif //SPACEGAME_EXPLOSIONBEHAVIOUR_H
