// This file is part of SpaceGame.
// Copyright (C) 2015 Miika Pelkonen
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

#ifndef __Resources_H_
#define __Resources_H_

#include "BlockMapping.h"
#include "Music.h"
#include "Sample.h"
#include "Texture.h"
#include "Font.h"

class Resources {
    friend class App;

public:
    class Samples {
        friend class Resources;

    public:
        Sample* const explosion;
        Sample* const missileCollision;
        Sample* const spaceshipAcceleration;
        Sample* const spaceshipCollision;
        Sample* const spaceshipExplosion;
        Sample* const spaceshipShoot;

    private:
        Samples();
        ~Samples();
    };

    class Tunes {
        friend class Resources;

    public:
        Music* const spacegame;

    private:
        Tunes();
        ~Tunes();
    };

    class Textures {
        friend class Resources;

    public:
        Texture* const animExplosion;
        Texture* const animRocketFire;
        Texture* const bg1;
        Texture* const bg2;
        Texture* const bomb;
        Texture* const light;
        Texture* const missile;
        Texture* const spaceship;
        Texture* const inventoryEquipableSlot;
        Texture* const inventorySelectedSlot;
        Texture* const inventorySlot;
        Texture* const laserCannon;
        Texture* const plasmaCannon;

    private:
        Textures();
        ~Textures();
    };

    class Other {
        friend class Resources;

    public:
        BlockMapping* const blockMapping;
        Font* const smallFont;

    private:
        Other();
        ~Other();
    };

    Samples* const samples;
    Tunes* const tunes;
    Textures* const textures;
    Other* const other;

private:
    Resources();
    ~Resources();
};

#endif //__Resources_H_
