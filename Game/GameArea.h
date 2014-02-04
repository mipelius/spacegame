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

#ifndef __GameArea_H_
#define __GameArea_H_

class Background;
class Music;

#include "Rect.h"
#include "Background.h"
#include "Music.h"

class GameArea {
private:
    Background* background_;
    Music* music_;
    Rect areaRect_;
public:
    GameArea(Background* background, Music* music, Rect areaRect);

    Music* getMusic();
    Background* getBackground();
    Rect getRect();
};


#endif //__GameArea_H_
